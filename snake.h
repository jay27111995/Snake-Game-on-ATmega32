/* 
 * Snake game on the Atmega32 
 *
 * Author: Jayendra Ellamathy 
 *
 * Date: 5th March 2021 
 *
 */


#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>


/* x and y co-ordinates for 8 * 8 matrix image */
typedef struct position {
    uint8_t x;
    uint8_t y;
} position_t;

/* Linked list struct to represent snake */
typedef struct snake {
    struct position pos;
    struct snake *next;
    struct snake *prev;
} snake_t;

/* Game directions */
typedef enum dir {
    UP, 
    DOWN, 
    LEFT, 
    RIGHT,
} dir_t;

extern void draw();


/* Creates the snake at the initial position at the start of the game */
void init_snake(); 

/* Add a snake body unit at the head position of the snake. Update snake when 
 * food is eaten */
void add_to_snake(uint8_t x, uint8_t y);

/* Draw the snake from the linked list on to the 8 * 8 matrix array - drawing */
void draw_snake(); 

/* Free up memory allocated for the snake */
void destroy_snake(); 

/* Init position of food at (4, 4)*/
void init_food();

/* generate food at random position. Ensures food does not collide with snake. */
void generate_food();

/* Draw the food onto the 8 * 8 matrix array - Drawing */
void draw_food();

/* Calculate the next move of the snake based on the direction */
void calculate_next_move(uint8_t *x, uint8_t *y); 

/* Check if the x and y co-ordinates collide with that of the snake body */
int check_valid_move(uint8_t x, uint8_t y);

/* Check current state and create the next state of the game */
int update_game();

/* Blink the drawing to indicate the game is over */
void blink_game_over();

/* Main game loop */
void game_loop();
