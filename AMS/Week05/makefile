all: simpletimer zombiewrap

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

simpletimer: simpletimer.c ../../ZDK04/libzdk04.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

zombiewrap: zombiewrap.c ../../ZDK04/libzdk04.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
