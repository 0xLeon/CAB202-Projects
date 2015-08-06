all: hello logo

LIBS=-lzdk01 -lncurses -lm -lrt
DIRS=-I../../ZDK01 -L../../ZDK01
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

hello: hello.c ../../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

logo: logo.c ../../ZDK01/libzdk01.a
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

