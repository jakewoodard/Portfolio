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
  Dllist needs_hydrogen; 
  Dllist needs_oxygen;
  Dllist needs_oh;
};

struct molecule {
  int h1_id;
  int h2_id;
  int o_id;
  pthread_cond_t *h1;
  pthread_cond_t *h2;
  pthread_cond_t *o;
  int counter;
};

struct molecule *initialize_m() {
  struct molecule *m;	
  m = malloc(sizeof(struct molecule));
  m->h1_id = -1;
  m->h2_id = -1;
  m->o_id = -1;
  m->counter = 0;
  m->h1 = new_cond();
  m->h2 = new_cond();
  m->o = new_cond();
  return m;
}

void *initialize_v(char *verbosity) {
  struct global_info *g;
  g = (struct global_info *) malloc(sizeof(struct global_info));
  g->lock = new_mutex();
  g->needs_hydrogen = new_dllist();
  g->needs_oxygen = new_dllist();
  g->needs_oh = new_dllist();
  return (void *) g;
}

void *hydrogen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  struct molecule *m = NULL;
  Dllist tmp;

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  if (dll_empty(g->needs_hydrogen)) {
	
  } else if (dll_empty(g->needs_oh)) {
	
  } else {
	m = initialize_m();
  }
  rv = Bond(m->h1_id, m->h2_id, m->o_id);

  // unlock mutex & return
  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}

void *oxygen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  struct molecule *m = NULL;
  Dllist tmp;

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  if (dll_empty(g->needs_oxygen)) {
	
  } else if (dll_empty(g->needs_oh)) {
	
  } else {
	m = initialize_m();
  }

  rv = Bond(m->h1_id, m->h2_id, m->o_id);

  pthread_mutex_unlock(g->lock);
  return (void *) rv;
}


