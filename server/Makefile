# Add cross compilation support

#ifneq ($(CROSS_COMPILE),)
#	CC ?= ${CROSS_COMPILE}
#else
#	CC = gcc
#endif

CC ?= $(CROSS_COMPILE)gcc
# Set flags
CFLAGS += -g -Wall -Werror -pthread

all: aesdsocket
	@echo "build done!"

aesdsocket: socket.o
	$(CC) $(CFLAGS) -lpthread socket.o -o aesdsocket

socket.o: socket.c
	$(CC) $(CFLAGS) -c socket.c -o socket.o

clean:
	rm -f *.o aesdsocket

check_memory: aesdsocket
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose \
	--log-file=valgrind-out.txt ./aesdsocket