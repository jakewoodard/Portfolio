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
  Dllist molecules; 
};

struct molecule {
  int h1_id;
  int h2_id;
  int o_id;
  pthread_cond_t *h1;
  pthread_cond_t *h2;
  pthread_cond_t *o;
};

struct molecule *m;

struct molecule *initialize_m() {
  struct molecule *m;	
  m = malloc(sizeof(struct molecule));
  m->h1_id = -1;
  m->h2_id = -1;
  m->o_id = -1;
  m->h1 = new_cond();
  m->h2 = new_cond();
  m->o = new_cond();
  return m;
}

void *initialize_v(char *verbosity) {
  struct global_info *g;
  g = (struct global_info *) malloc(sizeof(struct global_info));
  g->lock = new_mutex();
  g->molecules = new_dllist();
  return (void *) g;
}

void *hydrogen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv; int b = 0;
  struct molecule *m = NULL;
  Dllist tmp;

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  // if  list is empty
  if (dll_empty(g->molecules)) {
	m = initialize_m();
	m->h1_id = a->id;
	dll_append(g->molecules, new_jval_v((void *) m));
	// signal that h1 is ready and wait for h2
	pthread_cond_signal(m->h1);
	pthread_cond_wait(m->h2, g->lock);
  } else {
	// search list for molecule with h1 value,
	// without h2 value
	dll_traverse(tmp, g->molecules) {
	  m = ((struct molecule *)tmp->val.v);
	  if (m->h2_id == -1 && m->h1_id != -1) {
		m->h2_id = a->id;
		pthread_cond_signal(m->h2);
		b = 1;
	  }
	}
	// if not found then make new molecule and insert
	if (b == 0) {
	  m = initialize_m();
	  m->h1_id = a->id;
	  dll_append(g->molecules, new_jval_v((void *) m));
	  // signal that h1 is ready and wait for h2
	  pthread_cond_signal(m->h1);
	  pthread_cond_wait(m->h2, g->lock);
	}
  }

  // wait for oxygen, then call bond
  pthread_cond_wait(m->o, g->lock);
  rv = Bond(m->h1_id, m->h2_id, m->o_id);

  free(m);
  // unlock mutex & return
  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}

void *oxygen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  int id_h, id_h2, id_o;
  Dllist tmp; 

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  // add oxygen id to list 
  // and signal 
  dll_append(g->o, new_jval_i(a->id));
  if (m->o == -1) m->o = a->id;
  pthread_cond_signal(g->o_ready);

  // wait for hydrogen signal
  if (m->h1 == -1 || m->h2 == -1) 
	pthread_cond_wait(g->h_ready, g->lock);

  pthread_mutex_lock(g->lock);
  id_h = g->h->flink->val.i;
  id_h2 = g->h->flink->flink->val.i;
  id_o = g->o->flink->val.i;
  
  rv = Bond(m->h1, m->h2, m->o);

  // set molecule values back to -1
  // after recieving both signals confirming 
  // both hydrogens are done bonding
  pthread_cond_wait(g->reset, g->lock);
  pthread_cond_wait(g->reset, g->lock);

  pthread_mutex_lock(g->lock);
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
  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}


