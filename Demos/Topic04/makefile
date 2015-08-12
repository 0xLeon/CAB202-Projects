all: ex01

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

ex01: ex01.c ../../ZDK04/libzdk04.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
