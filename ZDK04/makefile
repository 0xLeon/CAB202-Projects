TARGET=libzdk04.a
FLAGS=-Wall -Werror -std=gnu99 -g

all: $(TARGET)

clean:
	rm $(TARGET)
	rm *.o

rebuild: clean all

$(TARGET): *.c *.h
	gcc -c *.c $(FLAGS)
	ar r $(TARGET) *.o
