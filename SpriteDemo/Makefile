all: SpriteDemo

LIBS=-lzdk01 -lncurses -lm -lrt
DIRS=-I../ZDK01 -L../ZDK01
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

SRC=blockhead.c one_square.c ten_squares.c sprite_demo.c
HDR=blockhead.h one_square.h ten_squares.h

SpriteDemo: $(SRC) $(HDR) ../ZDK01/libzdk01.a
	gcc $(FLAGS) $(SRC) -o $@ $(DIRS) $(LIBS) 
