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
  //pthread_mutex_lock(g->lock);

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
	  if (b == 0 && m->h2_id == -1 && m->h1_id != -1) {
		b = 1;
		m->h2_id = a->id;
		pthread_cond_signal(m->h2);
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

  // unlock mutex & return
  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}

void *oxygen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  int b = 0;
  Dllist tmp; 

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  //pthread_mutex_lock(g->lock);

  // if  list is empty
  if (dll_empty(g->molecules)) {
	m = initialize_m();
    m->o_id = a->id;
    dll_append(g->molecules, new_jval_v((void *) m));
  } else {
	// search for place for oxygen
	dll_traverse(tmp, g->molecules) {
	  m = ((struct molecule *)tmp->val.v);
	  // if spot is found insert oxygen atom
	  if (b == 0 && m->o_id == -1) {
		b = 1;
		m->o_id = a->id;
	  }
	}
	// if no open spot found, 
	// create new molecule & insert into dllist
	if (b == 0) {
	  m = initialize_m();
      m->o_id = a->id;
      dll_append(g->molecules, new_jval_v((void *) m));
	}
  }

  // signal that o is ready (twice!)
  pthread_cond_signal(m->o);
  pthread_cond_signal(m->o);

  // wait for h1 and h2 to be
  // ready for bonding
  pthread_cond_wait(m->h1, g->lock);
  pthread_cond_wait(m->h2, g->lock);

  rv = Bond(m->h1_id, m->h2_id, m->o_id);

  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}


