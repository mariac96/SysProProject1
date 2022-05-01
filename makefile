
CC=gcc

CFLAGS=-g 

all: vaccineMonitor

vaccineMonitor: main.o functions.o citizens.o country.o hash_funcs.o hashing.o skiplist.o virus.o
	$(CC) main.o functions.o citizens.o country.o hash_funcs.o hashing.o skiplist.o virus.o -o vaccineMonitor -lm

main.o: main.c functions.h hashing.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c citizens.h skiplist.h virus.h
	$(CC) $(CFLAGS) -c functions.c

citizens.o: citizens.c citizens.h hashing.h country.h virus.h
	$(CC) $(CFLAGS) -c citizens.c

country.o: country.c country.h
	$(CC) $(CFLAGS) -c country.c

hash_funcs.o: hash_funcs.c hash_funcs.h
	$(CC) $(CFLAGS) -c hash_funcs.c

hashing.o: hashing.c hashing.h citizens.h virus.h country.h
	$(CC) $(CFLAGS) -c hashing.c

skiplist.o: skiplist.c skiplist.h citizens.h
	$(CC) $(CFLAGS) -c skiplist.c

virus.o: virus.c virus.h hash_funcs.h hashing.h skiplist.h -lm
	$(CC) $(CFLAGS) -c virus.c

clean:
	rm -f *.o vaccineMonitor
