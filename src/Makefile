# generated binary name
BINARY=parallel-primes

# compiler info: which compiler to use and flags
CC=mpicc
CCFLAGS=-std=c99 -Wall -Werror

# build rules: the actual build rules for the program. Do not call locally.
all: main.o
	mkdir -p ../bin/ && $(CC) $(CCFLAGS) pprimes.o ppmw.o main.o -o ../bin/$(BINARY)

main.o: pprimes.o ppmw.o main.c 
	$(CC) $(CCFLAGS) -c main.c

pprimes.o: pprimes.c
	$(CC) $(CCFLAGS) -c pprimes.c

ppmw.o: ppmw.c
	$(CC) $(CCFLAGS) -c ppmw.c
