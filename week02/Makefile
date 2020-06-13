# COMP2521 19t1 ... lab 6

CC	= gcc
CFLAGS	= -Wall -Werror -g
LDFLAGS	=

.PHONY: all
all: sorter testQ

sorter:		sorter.o
sorter.o:	sorter.c
testQ:		testQ.o Queue.o
testQ.o:	testQ.c Queue.h
Queue.o:	Queue.c Queue.h

.PHONY: clean
clean:
	-rm -f sorter sorter.o
	-rm -f testQ testQ.o Queue.o

.PHONY: bad
bad: clean 
	mv sorter.c sorter.c.good
	mv sorter.c.bad sorter.c
	mv Queue.c Queue.c.good
	mv Queue.c.bad Queue.c

.PHONY: good
good: clean 
	mv sorter.c sorter.c.bad
	mv sorter.c.good sorter.c
	mv Queue.c Queue.c.bad
	mv Queue.c.good Queue.c


