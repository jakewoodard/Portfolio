
EXECUTABLES = \
       bin/hw \
       bin/print4 \
       bin/p4a \
       bin/p4b \
       bin/p4c \
       bin/p4d \
       bin/p4e \
       bin/iloop \

all: $(EXECUTABLES)

clean:
	rm -f core obj/* bin/* a.out

bin/hw: src/hw.c
	$(CC) -o bin/hw src/hw.c -lpthread

bin/print4: src/print4.c
	$(CC) -o bin/print4 src/print4.c -lpthread

bin/p4a: src/p4a.c
	$(CC) -o bin/p4a src/p4a.c -lpthread

bin/p4b: src/p4b.c
	$(CC) -o bin/p4b src/p4b.c -lpthread

bin/p4c: src/p4c.c
	$(CC) -o bin/p4c src/p4c.c -lpthread

bin/p4d: src/p4d.c
	$(CC) -o bin/p4d src/p4d.c -lpthread

bin/p4e: src/p4e.c
	$(CC) -o bin/p4e src/p4e.c -lpthread

bin/iloop: src/iloop.c
	$(CC) -o bin/iloop src/iloop.c -lpthread
