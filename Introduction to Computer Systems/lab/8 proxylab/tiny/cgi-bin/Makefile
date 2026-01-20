CC = gcc
CFLAGS = -O2 -Wall -I ..

all: adder pingpong repeater forwarder setblock

adder: adder.c
	$(CC) $(CFLAGS) -o adder adder.c

pingpong: pingpong.c
	$(CC) $(CFLAGS) -o pingpong pingpong.c
	
repeater: repeater.c
	$(CC) $(CFLAGS) -o repeater repeater.c
	
forwarder: forwarder.c
	$(CC) $(CFLAGS) -o forwarder forwarder.c ../csapp.c -lpthread
	
setblock: setblock.c
	$(CC) $(CFLAGS) -o setblock setblock.c ../csapp.c -lpthread

clean:
	rm -f adder pingpong repeater forwarder setblock *~
