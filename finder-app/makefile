# Set Compiler
CC = aarch64-none-linux-gnu-





# Set Flags
FLAGS = -g -Wall -I/

all: writer
	@echo "Build done!"

writer: writer.o
	$(CC)gcc $(FLAGS) writer.o -o writer

writer.o: writer.c
	$(CC)gcc $(FLAGS) -c writer.c -o writer.o

clean:
	rm -f *.o *.out writer
