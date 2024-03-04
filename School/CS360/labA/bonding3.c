#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "fields.h"
#include "dllist.h"
#include "bonding.h"

struct global_info {
  pthread_mutex_t *lock;
  pthread_cond_t *h_ready;
  pthread_cond_t *o_ready;
  pthread_cond_t *reset;
  Dllist h; // waiting hydrogen threads
  Dllist o; // waiting oxygren threads
};

struct molecule {
  int h1;
  int h2;
  int o;
};

struct molecule *m;

void *initialize_v(char *verbosity) {
  struct global_info *g;
  g = (struct global_info *) malloc(sizeof(struct global_info));
  g->lock = new_mutex();
  g->h_ready = new_cond();
  g->o_ready = new_cond();
  g->reset = new_cond();
  g->h = new_dllist();
  g->o = new_dllist();
  m = malloc(sizeof(struct molecule));
  m->h1 = -1;
  m->h2 = -1;
  m->o = -1;
  return (void *) g;
}

void *hydrogen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  //pthread_mutex_lock(g->lock);

  // if either list is empty, then 
  // add thread to waitlist & wait for signal

  if (dll_empty(g->h)) {
	dll_append(g->h, new_jval_i(a->id));
	if (m->h1 == -1) m->h1 = a->id;
	pthread_cond_wait(g->h_ready, g->lock);
  } else {
	dll_append(g->h, new_jval_i(a->id));
	if (m->h2 == -1) m->h2 = a->id;
	pthread_cond_signal(g->h_ready);
  }

  if (m->o == -1) pthread_cond_wait(g->o_ready, g->lock);

  // when at least 2 h and 1 o are ready, 
  // call bond and signal reset to 
  // oxygen thread
  pthread_mutex_lock(g->lock);
  rv = Bond(m->h1, m->h2, m->o);
  pthread_cond_signal(g->reset);

  // unlock mutex & return
  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}

void *oxygen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  int id_h, id_h2, id_o, count;
  Dllist tmp; 

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  //pthread_mutex_lock(g->lock);

  // add oxygen id to list 
  // and signal 
  dll_append(g->o, new_jval_i(a->id));
  if (m->o == -1) m->o = a->id;
  pthread_cond_signal(g->o_ready);

  // wait for hydrogen signal
  if (m->h1 == -1 || m->h2 == -1) 
	pthread_cond_wait(g->h_ready, g->lock);

  id_h = g->h->flink->val.i;
  id_h2 = g->h->flink->flink->val.i;
  id_o = g->o->flink->val.i;
  
  rv = Bond(m->h1, m->h2, m->o);

  // set molecule values back to -1
  // after recieving both signals confirming 
  // both hydrogens are done bonding
  pthread_cond_wait(g->reset, g->lock);
  pthread_cond_wait(g->reset, g->lock);

  if (m->h1 == id_h) dll_delete_node(g->h->flink);
  if (m->h2 == id_h2) dll_delete_node(g->h->flink);
  if (m->o == id_o) dll_delete_node(g->o->flink);

  if (dll_empty(g->h)) {
	m->h1 = -1;
	m->h2 = -1;
  } else {
	m->h1 = g->h->flink->val.i;
	if (g->h->flink->flink != NULL) {
	  m->h2 = g->h->flink->flink->val.i;
	} else {
	  m->h2 = -1;
	}
  }
  if (dll_empty(g->o)) {
	m->o = -1;
  } else {
	m->o = g->o->flink->val.i;
  }
  //pthread_mutex_unlock(g->lock);
  return (void *) rv;
}
