all: hello logo ex01 ex02 

LIBS=-lzdk02 -lncurses -lm
DIRS=-I../ZDK02 -L../ZDK02
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

ex01: ex01.c ../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

ex02: ex02.c ../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

hello: hello.c ../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

logo: logo.c ../ZDK02/libzdk02.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

