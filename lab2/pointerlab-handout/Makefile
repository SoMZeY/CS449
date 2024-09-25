#
# Makefile that builds btest and other helper programs for the CS:APP data lab
#
CC = gcc
CFLAGS = -O -Wall -Werror -Wno-error=uninitialized -m64 -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) $< -c

ptest: ptest.o pointer.o common.o common.h
	$(CC) $(CFLAGS) -Wno-unused-variable -o ptest ptest.c pointer.c common.c

clean:
	rm -f *.o ptest *~
	rm -rf *.dSYM

test: ptest
	./ptest
