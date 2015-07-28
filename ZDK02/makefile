TARGET=libzdk02.a
FLAGS=-Wall -Werror -std=gnu99

all: $(TARGET)

clean:
	rm $(TARGET)
	rm *.o

rebuild: clean all

$(TARGET): *.c *.h
	gcc -c *.c $(FLAGS)
	ar r $(TARGET) *.o
