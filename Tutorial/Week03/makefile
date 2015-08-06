all: animation

LIBS=-lzdk02 -lncurses -lm
DIRS=-I../ZDK02 -L../ZDK02
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

animation: animation.c ../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
