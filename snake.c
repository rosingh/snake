#include <stdio.h>
#include "SDL2/SDL.h"
#include "snake.h"


SDL_Window *window;
SDL_Renderer *renderer;

int main(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError("SDL_Init");
        return 1;
    }
    
    window = createWindow();
    if (window == NULL) {
        logSDLError("SDL_CreateWindow");
        return 1;
    }
    
    renderer = createRenderer(window);
    if (renderer == NULL) {
        logSDLError("SDL_CreateRenderer");
        return 1;
    }

    start_game();
}

void logSDLError(const char *msg) {
    printf("%s error: %s\n", msg, SDL_GetError());
}

SDL_Window* createWindow(void) {
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    return window;
}

SDL_Renderer* createRenderer(SDL_Window *window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return renderer;
}
