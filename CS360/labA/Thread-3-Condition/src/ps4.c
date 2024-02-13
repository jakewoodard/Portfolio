/* Adding two condition variables. Please see the lecture notes, as there is a subtle bug
   in this program. */

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
  pthread_cond_t *full;         /* The users wait on this when the queue is full. */
  pthread_cond_t *empty;        /* The printers wait on this when the queue is empty. */
} Buffer;
  
void initialize_v(Spq *s)
{
  Buffer *b;

  b = (Buffer *) malloc(sizeof(Buffer));
  b->b = (Job **) malloc(sizeof(Job *)*s->bufsize);
  b->head = 0;
  b->tail = 0;
  b->njobs = 0;
  b->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  b->full = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  b->empty = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  pthread_mutex_init(b->lock, NULL);
  pthread_cond_init(b->full, NULL);
  pthread_cond_init(b->empty, NULL);
  s->v = (void *) b;
}

void submit_job(Spq *s, Job *j)
{
  Buffer *b;

  b = (Buffer *) s->v;

  pthread_mutex_lock(b->lock);
  while(1) {
    if (b->njobs < s->bufsize) {
      b->b[b->head] = j;
      b->head = (b->head + 1) % s->bufsize;
      b->njobs++;
      if (b->njobs == 1) pthread_cond_signal(b->empty);   // New code: Signal the printers when the queue was empty.
      pthread_mutex_unlock(b->lock);
      return;
    } else {
      printf("%4ld: user %2d blocking because the queue is full\n", 
             time(0)-s->starttime, s->id);
      fflush(stdout);
      pthread_cond_wait(b->full, b->lock);               // New code: wait when the queue is full.
    }
  }
}


Job *get_print_job(Spq *s)
{
  Buffer *b;
  Job *j;

  b = (Buffer *) s->v;

  pthread_mutex_lock(b->lock);
  while(1) {
    if (b->njobs > 0) {
      j = b->b[b->tail];
      b->tail = (b->tail + 1) % s->bufsize;
      b->njobs--;
      if (b->njobs == s->bufsize-1) pthread_cond_signal(b->full);  // New code: wait when the queue is empty.
      pthread_mutex_unlock(b->lock);
      return j;
    } else {
      printf("%4ld: prnt %2d blocking because the queue is empty\n", 
             time(0)-s->starttime, s->id);
      fflush(stdout);
      pthread_cond_wait(b->empty, b->lock);  // New code: wait when the queue is empty.
    }
  }
}
