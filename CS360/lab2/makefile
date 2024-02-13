CC = gcc
INCLUDES = -I/home/jplank/cs360/include
CFLAGS = $(INCLUDES)
LIBDIR = /home/jplank/cs360/objs
LIBS = $(LIBDIR)/libfdr.a

EXECUTABLES = famtree

all: $(EXECUTABLES)

bin/l2p1: src/l2p1.c
    $(CC) $(CFLAGS) -o famtree famtree.c $(LIBS)

clean:
    rm core $(EXECUTABLES) *.o
~                                                                                                                                   
~                                      
