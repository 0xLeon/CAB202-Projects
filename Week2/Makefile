all: hello balls lines paddles hello_cab

LIBS=-lzdk01 -lncurses -lm
DIRS=-I../ZDK01 -L../ZDK01
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

hello: hello.c ../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

balls: balls.c ../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

lines: lines.c ../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

paddles: paddles.c ../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

hello_cab: hello_cab.c ../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
