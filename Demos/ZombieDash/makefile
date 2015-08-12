all: ZombieDash

LIBS=-lzdk04 -lncurses -lm -lrt
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

SRC=main.c game.c player.c zombie.c
HDR=game.h player.h zombie.h

ZombieDash: $(SRC) $(HDR) ../../ZDK04/libzdk04.a
	gcc $(FLAGS) $(SRC) -o $@ $(DIRS) $(LIBS) 
