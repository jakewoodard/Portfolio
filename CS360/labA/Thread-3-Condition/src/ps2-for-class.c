#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "printqsim.h"

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
  /* Write this */
}

/* The printers still do nothing. */

Job *get_print_job(Spq *s)
{
  return NULL;
}
