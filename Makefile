# Name: Aidan Bush
# Course: 464
# Date: Oct 5

CC=msp430-gcc
CFLAGS= -mmcu=msp430f1611 -std=c99 -Wall -g

.PHONY: all clean

all: Image

# testCodes: testCodes.c codes.o

Image: Image.c uart.o codes.o radio.o

uart.o: uart.c uart.h

codes.o: codes.c codes.h

radio.o: radio.c radio.h

clean:
	$(RM) Image *.o

