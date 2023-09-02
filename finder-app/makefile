# Set Compiler
CC=gcc





# Set Flags
FLAGS = -g -Wall -I/

all: writer
	@echo "Build done!"

writer: writer.o
	$(CC) $(FLAGS) writer.o -o writer

writer.o: writer.c
	$(CC) $(FLAGS) -c writer.c -o writer.o

clean:
	rm -f *.o *.out writer
