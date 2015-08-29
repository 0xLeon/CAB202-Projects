all: release

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -Wall -Werror -D_XOPEN_SOURCE=500

clean:
	rm *.exe

rebuild: clean release

drebuild: clean debug

debug: *.c *.h ../../ZDK04/libzdk04.a
	gcc -g $(FLAGS) -DGAME_DEBUG $(EXTRAFLAGS) *.c -o megamaniac $(DIRS) $(LIBS) 

release: *.c *.h ../../ZDK04/libzdk04.a
	gcc $(FLAGS) $(EXTRAFLAGS) *.c -o megamaniac $(DIRS) $(LIBS) 
