#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "snake.h"
#include "snake_data.h"

cell grid[NUM_CELLS_Y][NUM_CELLS_X];
snake_t *snake;
SDL_Rect snake_food;

void start_game(void)
{
    SDL_Event e;
    bool quit = false;
    bool init = false;

    init_game();
    render_frame();

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
            {
                if (init)
                {
                    init_game();
                }
                game_loop();
                init = true;
                break;
            }
        }
    }
}

void init_game(void)
{
    srand(time(NULL));
    for (int i = 0; i < NUM_CELLS_Y; ++i)
    {
        for (int j = 0; j < NUM_CELLS_X; ++j)
        {
            grid[i][j].is_occupied = false;
            grid[i][j].is_food = false;
        }
    }

    for (int i = 0; i < NUM_CELLS_X; ++i)
    {
        grid[0][i].is_occupied = true;
        grid[NUM_CELLS_Y - 1][i].is_occupied = true;
    }

    for (int i = 0; i < NUM_CELLS_Y; ++i)
    {
        grid[i][0].is_occupied = true;
        grid[i][NUM_CELLS_X - 1].is_occupied = true;
    }

    snake = init_snake();
    int food_x;
    int food_y;
    do
    {
        food_x = (rand() % 22) + 1;
        food_y = (rand() % 22) + 1;
    } while (grid[food_y][food_x].is_occupied);

    grid[food_y][food_x].is_food = true;
    snake_food.x = food_x * GRID_SIZE + 1;
    snake_food.y = food_y * GRID_SIZE + 1;
    snake_food.w = snake_food.h = SEGMENT_SIZE;
}

void game_loop(void)
{
    SDL_Event e;
    bool quit = false;
    e_move movement = MOVE_RIGHT;

    Uint32 last_tick = SDL_GetTicks();
    
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN) 
            {
                movement = handle_event(e, movement);
                break;
            }
        }
        if (!update_snake(movement))
        {
            break;
        }
        render_frame();

        Uint32 fps = SDL_GetTicks() - last_tick;
        if (fps < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps);
        }
        last_tick = SDL_GetTicks();
    }
    free_snake();
}

e_move handle_event(SDL_Event e, e_move movement)
{
    switch(e.key.keysym.sym)
    {
        case SDLK_UP:
            if (movement != MOVE_DOWN)
            {
                movement = MOVE_UP;
            }
            break;
        case SDLK_DOWN:
            if (movement != MOVE_UP)
            {
                movement = MOVE_DOWN;
            }
            break;
        case SDLK_LEFT:
            if (movement != MOVE_RIGHT)
            {
                movement = MOVE_LEFT;
            }
            break;
        case SDLK_RIGHT:
            if (movement != MOVE_LEFT)
            {
                movement = MOVE_RIGHT;
            }
            break;
        default:
            break;
    }
    return movement;
}

snake_t *init_snake(void)
{
    snake_t *new_snake = (snake_t *) malloc(sizeof(snake));
    if (NULL == new_snake)
    {
        exit(EXIT_FAILURE);
    }
    snake_part *new_snake_head = (snake_part *) malloc(sizeof(snake_part));
    if (NULL == new_snake_head)
    {
        exit(EXIT_FAILURE);
    }
    new_snake_head->snake_segment.x = 276;
    new_snake_head->snake_segment.y = 451;
    new_snake_head->snake_segment.w = new_snake_head->snake_segment.h = SEGMENT_SIZE;
    new_snake->head = new_snake_head;
    snake_part *cursor = new_snake->head;
    int current_snake_x_cell = cursor->snake_segment.x / GRID_SIZE;
    int current_snake_y_cell = cursor->snake_segment.y / GRID_SIZE;
    grid[current_snake_y_cell][current_snake_x_cell].is_occupied = true;

    for (int i = 0; i < 4; ++i)
    {
        cursor->next = (snake_part *) malloc(sizeof(snake_part));
        if (NULL == cursor->next)
        {
            exit(EXIT_FAILURE);
        }
        cursor->next->snake_segment.x = cursor->snake_segment.x - 25;
        cursor->next->snake_segment.y = cursor->snake_segment.y;
        cursor->next->snake_segment.w = cursor->next->snake_segment.h = SEGMENT_SIZE;
        current_snake_x_cell = cursor->next->snake_segment.x / GRID_SIZE;
        current_snake_y_cell = cursor->next->snake_segment.y / GRID_SIZE;
        grid[current_snake_y_cell][current_snake_x_cell].is_occupied = true;
        cursor = cursor->next;
    }
    cursor->next = NULL;
    new_snake->tail = cursor;
    return new_snake;
}

bool update_snake(e_move movement)
{
    int oldx = snake->head->snake_segment.x;
    int oldy = snake->head->snake_segment.y;
    switch(movement)
    {
        case MOVE_UP:
            snake->head->snake_segment.y -= GRID_SIZE;
            break;

        case MOVE_DOWN:
            snake->head->snake_segment.y += GRID_SIZE;
            break;

        case MOVE_LEFT:
            snake->head->snake_segment.x -= GRID_SIZE;
            break;

        case MOVE_RIGHT:
            snake->head->snake_segment.x += GRID_SIZE;
            break;
    }

    int tail_x_cell = snake->tail->snake_segment.x / GRID_SIZE;
    int tail_y_cell = snake->tail->snake_segment.y / GRID_SIZE;
    grid[tail_y_cell][tail_x_cell].is_occupied = false;

    int head_x_cell = snake->head->snake_segment.x / GRID_SIZE;
    int head_y_cell = snake->head->snake_segment.y / GRID_SIZE;
    if (grid[head_y_cell][head_x_cell].is_occupied)
    {
        return false;
    }
    else
    {
        grid[head_y_cell][head_x_cell].is_occupied = true;
        if (grid[head_y_cell][head_x_cell].is_food)
        {
            snake_part *new_tail = (snake_part *) malloc(sizeof(snake_part));
            if (NULL == new_tail)
            {
                exit(EXIT_FAILURE);
            }
            grid[tail_y_cell][tail_x_cell].is_occupied = true;
            new_tail->snake_segment.w = new_tail->snake_segment.h = SEGMENT_SIZE;
            new_tail->next = NULL;
            snake->tail->next = new_tail;
            snake->tail = new_tail;
            grid[head_y_cell][head_x_cell].is_food = false;
            regen_food();
        }
    }

    for (snake_part *cursor = snake->head->next; cursor != NULL; cursor = cursor->next)
    {
        int tempx = cursor->snake_segment.x;
        int tempy = cursor->snake_segment.y;
        cursor->snake_segment.x = oldx;
        cursor->snake_segment.y = oldy;
        oldx = tempx;
        oldy = tempy;
    }
    return true;
}

void render_frame(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 25, 25, 255);
    SDL_RenderFillRect(renderer, &snake_food);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render_borders();
    

    for (snake_part *cursor = snake->head; cursor != NULL; cursor = cursor->next)
    {
        SDL_RenderFillRect(renderer, &(cursor->snake_segment));
    }

    SDL_RenderPresent(renderer);
}

void render_borders(void)
{
    SDL_Rect border_top;
    SDL_Rect border_left;
    SDL_Rect border_right;
    SDL_Rect border_bottom;

    border_top.x = 0;
    border_top.y = 0;
    border_top.w = WIDTH - GRID_SIZE;
    border_top.h = GRID_SIZE;

    border_left.x = 0;
    border_left.y = GRID_SIZE;
    border_left.w = GRID_SIZE;
    border_left.h = HEIGHT;

    border_right.x = WIDTH - GRID_SIZE;
    border_right.y = 0;
    border_right.w = GRID_SIZE;
    border_right.h = HEIGHT;

    border_bottom.x = GRID_SIZE;
    border_bottom.y = HEIGHT - GRID_SIZE;
    border_bottom.w = WIDTH - GRID_SIZE;
    border_bottom.h = GRID_SIZE;

    SDL_RenderFillRect(renderer, &border_top);
    SDL_RenderFillRect(renderer, &border_left);
    SDL_RenderFillRect(renderer, &border_right);
    SDL_RenderFillRect(renderer, &border_bottom);
}

void regen_food(void)
{
    bool all_occupied = true;
    for (int i = 0; i < NUM_CELLS_Y; ++i)
    {
        for (int j = 0; j < NUM_CELLS_X; ++j)
        {
            if (grid[i][j].is_occupied)
            {
                all_occupied = false;
                break;
            }
        }
    }

    if (all_occupied)
    {
        return;
    }

    int food_x;
    int food_y;
    do
    {
        food_x = (rand() % 22) + 1;
        food_y = (rand() % 22) + 1;
    } while (grid[food_y][food_x].is_occupied);

    grid[food_y][food_x].is_food = true;
    snake_food.x = food_x * GRID_SIZE + 1;
    snake_food.y = food_y * GRID_SIZE + 1;
}

void free_snake(void)
{
    snake_part *cursor = snake->head;
    if (NULL == cursor)
        return;

    for ( ; cursor->next != NULL; cursor = cursor->next)
    {
        free(cursor);
    }
    free(cursor);
    free(snake);
}
