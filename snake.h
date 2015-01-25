#ifndef SNAKE_H
#define SNAKE_H

#define WIDTH 600
#define HEIGHT 600
#define FRAMES_PER_SECOND 15

void start_game(void);
void logSDLError(const char *msg);
SDL_Window* createWindow(void);
SDL_Renderer* createRenderer(SDL_Window *window);

extern SDL_Window *window;
extern SDL_Renderer *renderer;

#endif
