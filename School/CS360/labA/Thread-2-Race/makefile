
EXECUTABLES = bin/preempt1 \
              bin/preempt2 \
              bin/preempt3 \
              bin/race1 \
              bin/race2 \
              bin/race3 \
              bin/mutex_example_1 \
              bin/mutex_example_2 \
              bin/mutex_example_3 \


CC = gcc

all: $(EXECUTABLES)

clean:
	rm -f core a.out bin/*

bin/preempt1: src/preempt1.c
	$(CC) -o bin/preempt1 src/preempt1.c -lpthread

bin/preempt2: src/preempt2.c
	$(CC) -o bin/preempt2 src/preempt2.c -lpthread

bin/preempt3: src/preempt3.c
	$(CC) -o bin/preempt3 src/preempt3.c -lpthread

bin/race1: src/race1.c
	$(CC) -o bin/race1 src/race1.c -lpthread

bin/race2: src/race2.c
	$(CC) -o bin/race2 src/race2.c -lpthread

bin/race3: src/race3.c
	$(CC) -o bin/race3 src/race3.c -lpthread

bin/mutex_example_1: src/mutex_example_1.c
	gcc -o bin/mutex_example_1 src/mutex_example_1.c -lpthread

bin/mutex_example_2: src/mutex_example_2.c
	gcc -o bin/mutex_example_2 src/mutex_example_2.c -lpthread

bin/mutex_example_3: src/mutex_example_3.c
	gcc -o bin/mutex_example_3 src/mutex_example_3.c -lpthread


