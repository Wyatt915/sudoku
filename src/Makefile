CC=g++
CFLAGS= -std=c++14 -O3

all: solve

solve: sudokusolve.cpp structure.hpp
	$(CC) $(CFLAGS) sudokusolve.cpp -o solve

clean:
	rm -rf *.o

clean-win:
	del /S *.o
