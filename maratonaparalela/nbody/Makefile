FLAGS=-O3 -Wall
RM=rm -f
CC=gcc

EXEC=nbody

all: $(EXEC)

$(EXEC):
	$(CXX) $(CXXFLAGS) -fopenmp $(EXEC).c -c -o $(EXEC).o
	$(CXX) $(CXXFLAGS) $(EXEC).o -fopenmp -o $(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
