/* 
 * Snake game on the Atmega32 
 *
 * Author: Jayendra Ellamathy 
 *
 * Date: 5th March 2021 
 *
 */


#include "snake.h"


extern uint8_t drawing[8][8];

/* Snake is a linked list starting with HEAD snake_head */
snake_t *snake_head = NULL;
snake_t *snake_tail = NULL;

/* Current position of the food */
position_t food;

/* Current direction of the game */
dir_t direction;

/* Creates the snake at the initial position at the start of the game */
void init_snake() 
{
    /* Initialize snake with head + 1 body unit */
    snake_head = (snake_t *) malloc(sizeof(snake_t));
    snake_tail = (snake_t *) malloc(sizeof(snake_t));
    
    /* Start snake game with snake at (2, 2) and (2, 3) */
    snake_head->pos.x = 2;
    snake_head->pos.y = 2;
    snake_head->next = snake_tail;
    snake_head->prev = NULL;
    
    snake_tail->pos.x = 3;
    snake_tail->pos.y = 2;
    snake_tail->next = NULL; 
    snake_tail->prev = snake_head;
}

/* Add a snake body unit at the head position of the snake. Update snake when 
 * food is eaten */
void add_to_snake(uint8_t x, uint8_t y)
{
    snake_t *snake;

    snake = (snake_t *) malloc(sizeof(snake_t));
        
    snake_head->prev = snake;
    snake->next = snake_head;
    snake->prev = NULL;
    snake_head = snake;
}

/* Draw the snake from the linked list on to the 8 * 8 matrix array - drawing */
void draw_snake() 
{
    snake_t *snake;

    snake = snake_head;

    while (snake != NULL) {
        drawing[snake->pos.x][snake->pos.y] = 1;
        snake = snake->next;
    }
}

/* Free up memory allocated for the snake */
void destroy_snake() 
{
    snake_t *snake;
    snake_t *next;

    snake = snake_head;
    next = snake->next;

    while (snake != NULL) {
        free(snake);
        snake = next;
        next = snake->next;
    }

    snake_head = NULL;
    snake_tail = NULL;
}

/* Start game with food at the (4, 4) */
void init_food()
{
    food.x = 4;
    food.y = 4;
}

/* generate food at random position. Ensures food does not collide with snake. */
void generate_food()
{
    uint8_t x, y = 0;

    while (1) {
        x = rand() % 8;
        y = rand() % 8;

        if (drawing[x][y] == 1) { 
            //Snake body - Hence generate at another location 
        } else {
            food.x = x;
            food.y = y;
            break;
        }
    } 
}

/* Draw the food onto the 8 * 8 matrix array - Drawing */
void draw_food() 
{
    drawing[food.x][food.y] = 1;
}

/* Calculate the next move of the snake based on the direction */
void calculate_next_move(uint8_t *x, uint8_t *y) 
{
    //Calculate movement by 1 position 
    switch (direction) 
    {
        case UP:
            if ((*y - 1) < 0) { //If you go over edges then round up the value 
                *y = 7;
            } else {
                *y = *y - 1;
            }
            break;
        case DOWN:
            if ((*y + 1) > 7) {
                *y = 0;
            } else {
                *y = *y + 1;
            }
            break;
        case LEFT:
            if ((*x - 1) < 0) {
                *x = 7;
            } else {
                *x = *x - 1;
            }
            break;
        case RIGHT:
            if ((*x + 1) > 7) {
                *x = 0;
            } else {
                *x = *x + 1;
            }
            break;
        default:
            return;
    }
}

/* Check if the x and y co-ordinates collide with that of the snake body */
int check_valid_move(uint8_t x, uint8_t y) 
{
    snake_t *snake;

    snake = snake_head;

    while (snake != NULL) {
        if ((snake->pos.x == x) && (snake->pos.y == y)) {
            return -1;
        }
        snake = snake->next;
    }

    return 0;
}

/* Update game: Calculate the next state of the game - Next snake position, food position, game over ? 
 * RETURN: -1 if next move ends game, 0 for a valid move */
int update_game() 
{
    snake_t *snake;
    uint8_t x,y = 0;


    //Get the current value of head 
    x = snake_head->pos.x;
    y = snake_head->pos.y;

    snake = snake_tail;
    
    //Kick out the last value 
    while (snake->prev != NULL)
    {
        snake->pos.x = snake->prev->pos.x;
        snake->pos.y = snake->prev->pos.y;

        snake = snake->prev;
    }
   
    //Calculate next move based on the direction 
    calculate_next_move(&x, &y);
    
    if (-1 == check_valid_move(x, y)) {
        return -1;
    }

    //Check if next position is food, append the snake if so 
    if ((food.x == x) && (food.y == y)) 
    {
        add_to_snake(x, y);
        generate_food(); //Since food is consumed, now add food in new location 
    }

    //Update the position of the head 
    snake_head->pos.x = x;
    snake_head->pos.y = y;
    
    return 0;
}

/* Blink the drawing to indicate the game is over */
void blink_game_over()
{
    uint8_t i = 0;
    
    for (i = 0; i < 6; i++) 
    {
        refresh_drawing();
        
        if (i % 2 == 0) {
            draw_snake();
            draw_food();
        }

        draw();
        
        _delay_ms(500);
    }
}

/* Main game loop */
void game_loop()
{
    /* Initialize game */
    init_snake();
    init_food();

    while(1) 
    {
        refresh_drawing();

        draw_snake();
        draw_food();

        draw();

        if (-1 == update_game()) {
            blink_game_over();
            destroy_snake();
            return;
        }

        _delay_ms(200); 
    }
}
