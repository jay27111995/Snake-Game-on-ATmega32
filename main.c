/* 
 * Snake game on the Atmega32 
 *
 * Author: Jayendra Ellamathy 
 *
 * Date: 5th March 2021 
 *
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "snake.h"


extern dir_t direction;

/* Array of PORTD output values. Each value drives 1 row of the LED matrix */
volatile uint8_t framebuffer[8]; 

/* Current image we display on the LED matrix */
uint8_t drawing[8][8] = {0};


/* Interrupt service routine to handle one row at a time. 
 * Within 8 interrupts, all 8 rows of the LED matrix will be covered. */
ISR(TIMER0_OVF_vect) //Timer based interrupt - occurs every 0 to 255 count of timer  
{
    static uint8_t digit;

    PORTB = 0b11111111; //Clear all Rows  
    PORTB = ~(0b00000001 << digit);  //Set current Row 

    //Set columns 
    PORTD = (framebuffer[digit] & 0b11110000) | 0b00001111;
    PORTF = (framebuffer[digit] & 0b00000011) | ((framebuffer[digit] & 0b00001100) << 2);

    digit++;
    digit = digit % 8; //Repeat from start 
}


/* Button interrupts:
 * Each button is tied up with an external interrupt.
 *
 * INT0: PD0: DOWN 
 * INT1: PD1: UP
 * INT2: PD2: LEFT 
 * INT3: PD3; RIGHT */
ISR(INT0_vect)
{
    direction = DOWN;
}

ISR(INT1_vect)
{
    direction = UP;
}

ISR(INT2_vect)
{
    direction = LEFT;
}

ISR(INT3_vect)
{
    direction = RIGHT;
}

void init(void)
{
    //Declare pins as output pins for LED and input for buttons   
    DDRD = 0b11110000;   
    DDRB = 0b11111111;   
    DDRF = 0b00110011;

    PORTD = 0b00001111; //Pull-up to HIGH 
    
    /* Set interrupt mask for buttons */
    EIMSK |= (1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3); 
    EICRA = (1<<ISC01 | 1<<ISC00) | (1<<ISC11 | 1<<ISC10) | (1<<ISC21 | 1<<ISC20) | (1<<ISC31 | 1<<ISC30);  /* Trigger on rising edge */

    // we start with 
    // normal operation
    // clk/1024 prescaler (about 8 kHz)  
    // interrupt on Timer 0 overflow

    TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0)
        | (0 << WGM01)  | (0 << WGM00);   
    TCCR0B = (0 << WGM02) 
        | (0 << CS02)   | (1 << CS01)   | (1 << CS00); //Clock prescaler: No pre-scaling - 001  
    TIMSK0 = (0 << OCIE0B) | (0 << OCIE0A) | (1 << TOIE0); 

    UDIEN = 0; // USB interrupts are not handled - disable! //Glitch 
    sei();     // allow interrupts globally //Set interrupts - Dont forget!!
}

/* Clear the 8 * 8 matrix */
void refresh_drawing()
{
    memset(drawing, 0, (sizeof(uint8_t) * 8 * 8));
}

/* Function fills framebuffer from the image (8 * 8 Matrix) - drawing */
void draw() 
{
    uint8_t i, j = 0;

    for (i = 0; i < 8; i++) 
    {
        framebuffer[i] = 0; //Clear the row for each drawing 
        
        for (j = 0; j < 8; j++) 
        {
            if (drawing[i][j] == 1) {
                framebuffer[i] |= (1 << (7 - j));
            }
        }
    }
}


int main(void)
{
    /* Initialize HW/interrupts  */
    init(); 
   
    while(1)
    {
        game_loop();
    }
}

