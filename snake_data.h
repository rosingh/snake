#ifndef SNAKE_DATA_H
#define SNAKE_DATA_H

#include <stdbool.h>

#define GRID_SIZE 25
#define SEGMENT_SIZE 23
#define NUM_CELLS_X WIDTH / GRID_SIZE
#define NUM_CELLS_Y HEIGHT / GRID_SIZE

typedef struct snake_part
{
    SDL_Rect snake_segment;
    struct snake_part *next;
} snake_part;

typedef struct snake_t
{
    snake_part *head;
    snake_part *tail;
} snake_t;

typedef struct cell
{
    bool is_occupied;
    bool is_food;
} cell;

typedef enum
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
} e_move;

void init_game(void);
bool update_snake(e_move movement);
snake_t *init_snake(void);
e_move handle_event(SDL_Event e, e_move movement);
void game_loop(void);
void render_frame(void);
void render_borders(void);
void regen_food(void);
void free_snake(void);

#endif
