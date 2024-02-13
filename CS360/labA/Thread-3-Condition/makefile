CC = gcc 

EXECUTABLES = bin/ps1 bin/ps2 bin/ps3 bin/ps4 bin/ps5

all: $(EXECUTABLES)

clean:
	rm -f core obj/* bin/*

obj/printqsim.o: src/printqsim.c include/printqsim.h
	$(CC) -Iinclude -c -o obj/printqsim.o src/printqsim.c

bin/ps1: src/ps1.c include/printqsim.h obj/printqsim.o
	$(CC) -Iinclude -o bin/ps1 obj/printqsim.o src/ps1.c -lpthread

bin/ps2: src/ps2.c include/printqsim.h obj/printqsim.o
	$(CC) -Iinclude -o bin/ps2 obj/printqsim.o src/ps2.c -lpthread

bin/ps3: src/ps3.c include/printqsim.h obj/printqsim.o
	$(CC) -Iinclude -o bin/ps3 obj/printqsim.o src/ps3.c -lpthread

bin/ps4: src/ps4.c include/printqsim.h obj/printqsim.o
	$(CC) -Iinclude -o bin/ps4 obj/printqsim.o src/ps4.c -lpthread

bin/ps5: src/ps5.c include/printqsim.h obj/printqsim.o
	$(CC) -Iinclude -o bin/ps5 obj/printqsim.o src/ps5.c -lpthread
