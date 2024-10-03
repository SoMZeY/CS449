CC = gcc
CFLAGS = -O0 -g -Wall -Werror

all: qtest

queue.o: queue.c queue.h harness.h
	$(CC) $(CFLAGS) -c queue.c 

qtest_timed: qtest.c report.c console.c harness.c queue.o
	$(CC) $(CFLAGS) -fcommon -o qtest qtest.c report.c console.c harness.c queue.o 

qtest: qtest.c report.c console.c harness.c queue.o
	$(CC) $(CFLAGS) -DDEBUG_TIME=1 -fcommon -o qtest qtest.c report.c console.c harness.c queue.o 


test: qtest_timed driver.py
	chmod +x driver.py
	./driver.py -w

clean:
	rm -f *.o *~ qtest
	rm -rf *.dSYM
	(cd traces; rm -f *~)

