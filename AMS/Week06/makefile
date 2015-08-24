all: spaceinvaders

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

spaceinvaders: spaceinvaders.c game.h ../../ZDK04/libzdk04.a
	gcc $(FLAGS) game.h $@.c -o $@ $(DIRS) $(LIBS) 
 
