# GNU Makefile

CC = gcc 
CCFLAGS = -Wall -O3 -pthread
LDFLAGS = -pthread
TARGET = main

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@ 


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