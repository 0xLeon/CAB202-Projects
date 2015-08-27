all: release

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean release

drebuild: clean debug

debug: *.c *.h ../../ZDK04/libzdk04.a
	gcc $(FLAGS) -g -DGAME_DEBUG *.c -o megamaniac $(DIRS) $(LIBS) 

release: *.c *.h ../../ZDK04/libzdk04.a
	gcc $(FLAGS) *.c -o megamaniac $(DIRS) $(LIBS) 
