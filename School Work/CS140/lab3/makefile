
CC = g++
CFLAGS = -g -std=c++11
BIN = Prime1 Prime2 Prime3 Prime4

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $* $< 

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BIN)
