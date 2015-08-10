all: drawrect fillrect insiderect drawellipse zombieracer

LIBS=-lzdk02 -lncurses -lm
DIRS=-I../../ZDK02 -L../../ZDK02
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

drawrect: drawrect.c ../../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

fillrect: fillrect.c ../../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

insiderect: insiderect.c ../../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

drawellipse: drawellipse.c ../../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

zombieracer: zombieracer.c ../../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
