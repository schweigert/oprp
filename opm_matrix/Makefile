# GNU Makefile

CC = gcc 
CCFLAGS = -Wall -O3
LDFLAGS = -fopenmp
LCFLAGS = -fopenmp
TARGET = main

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(LCFLAGS)

%: %.o
	$(CC) $(LDFLAGS) $^ $(LCFLAGS) -o $@ 


all: $(TARGET)

# Dependencias

main: matrix.o main.c
matrix.o: matrix.c matrix.h

clean:
	rm -f *.o *~ $(TARGET) pg

pg:
	gcc -pg main.c matrix.c -o pg

gprof:
	gprof pg gmon.out