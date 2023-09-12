# Add cross compilation support




# Set flags
CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror -pthread
TARGET ?= aesdsocket
LDFLAGS ?= -lpthread -lrt

all: $(TARGET)
	@echo "build done!"

$(TARGET): socket.o
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) socket.o -o aesdsocket 

socket.o: socket.c
	$(CC) $(CFLAGS) -c socket.c -o socket.o

clean:
	rm -f *.o aesdsocket

check_memory: aesdsocket
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose \
	--log-file=valgrind-out.txt ./aesdsocket
