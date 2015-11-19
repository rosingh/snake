CC = clang
SDL_LIB = -L/usr/local/lib -lSDL2
CFLAGS = -Wall -Werror -O2 -std=c99 -I/usr/local/include
LDFLAGS = $(SDL_LIB)
OBJS = snake.o snake_game.o

all: snake

snake: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) -c $<

snake_game.o: snake_game.c snake_data.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o snake
