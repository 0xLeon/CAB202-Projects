all: release

LIBS=-lzdk04 -lncurses -lm
DIRS=-I../../ZDK04 -L../../ZDK04
FLAGS=-std=gnu99 -Wall -D_XOPEN_SOURCE=500

clean:
	-@rm *.exe *.stackdump 2>/dev/null
	-@rm *.alignments *.asmcons *.barriers *.dfinish *.dfinit *.dwarf2 *.expand *.final *.into_cfglayout *.ira *.jump *.jump2 *.mach *.mode_sw *.nothrow *.outof_cfglayout *.pro_and_epilogue *.reginfo *.reload *.shorten *.split1 *.split2 *.stack *.vregs 2>/dev/null

rebuild: clean release

drebuild: clean debug

debug: *.c *.h ../../ZDK04/libzdk04.a
	gcc -g $(FLAGS) -DGAME_DEBUG $(EXTRAFLAGS) *.c -o megamaniac $(DIRS) $(LIBS) 

release: *.c *.h ../../ZDK04/libzdk04.a
	gcc -s $(FLAGS) $(EXTRAFLAGS) *.c -o megamaniac $(DIRS) $(LIBS) 
