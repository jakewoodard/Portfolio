#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "printqsim.h"

/* We're going to define a Buffer here, and store it in the void *.
   It has a fixed size array of job pointers, plus head, tail, and
   njobs variables to define a queue of jobs within the buffer.
   Finally, there is a mutex to protect access to the buffer. */

typedef struct {
  Job **b;       
  int head;
  int tail;
  int njobs;
  pthread_mutex_t *lock;
} Buffer;
  
/* Initialize_v() sets up the buffer and then copies the pointer
   to the buffer to the (void *). */

void initialize_v(Spq *s)
{
  Buffer *b;

  b = (Buffer *) malloc(sizeof(Buffer));
  b->b = (Job **) malloc(sizeof(Job *)*s->bufsize);
  b->head = 0;
  b->tail = 0;
  b->njobs = 0;
  b->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(b->lock, NULL);
  s->v = (void *) b;
}

/* This puts a job onto the buffer if there's room.
   If there's not room, it simply kills the thread. */

void submit_job(Spq *s, Job *j)
{
  Buffer *b;

  b = (Buffer *) s->v;

  while(1) {
    pthread_mutex_lock(b->lock);
    if (b->njobs < s->bufsize) {
      b->b[b->head] = j;
      b->head = (b->head + 1) % s->bufsize;
      b->njobs++;
      pthread_mutex_unlock(b->lock);
      return;
    } else {
      pthread_mutex_unlock(b->lock);
      printf("%4ld: user %2d -- the queue is full -- exiting\n", time(0)-s->starttime, s->id);
      fflush(stdout);
      pthread_exit(NULL);
    }
  }
}

/* The printers still do nothing. */

Job *get_print_job(Spq *s)
{
  return NULL;
}
