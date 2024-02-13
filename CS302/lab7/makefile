# makefile: version 2
# enter: 
#  "make" or "make Mazemake" for compiling Mazemake
#  "make Mazesolve" for compiling Mazesolve

CC = g++
CFLAGS = -g -Wall

mazemake: mazemake.o
	$(CC) $(CFLAGS) -o mazemake mazemake.o

mazesolve: mazesolve.o
	$(CC) $(CFLAGS) -o mazesolve mazesolve.o

mazemake.o: mazemake.cpp
	$(CC) $(CFLAGS) -c mazemake.cpp

mazesolve.o: mazesolve.cpp 
	$(CC) $(CFLAGS) -c mazesolve.cpp

clean:
	rm -f mazemake.o
	rm -f mazesolve.o
	rm -f maze.txt
	rm -f path.txt
