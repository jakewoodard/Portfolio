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
  m->h1 = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  pthread_cond_init(m->h1, NULL);
  m->h2 = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  pthread_cond_init(m->h2, NULL);
  m->o = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  pthread_cond_init(m->o, NULL);
  return m;
}

void *initialize_v(char *verbosity) {
  struct global_info *g;
  g = (struct global_info *) malloc(sizeof(struct global_info));
  g->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(g->lock, NULL);
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
  int hydrogen1, hydrogen2, oxygen;
  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  if (!(dll_empty(g->needs_hydrogen))) {
	m = (struct molecule *) g->needs_hydrogen->flink->val.v;
	if (m->h1_id == -1) {
	  m->h1_id = a->id;
	  dll_delete_node(g->needs_hydrogen->flink);
	  dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	  pthread_cond_wait(m->h1, g->lock);
	} else if (m->h2_id == -1) {
	  m->h2_id = a->id;
	  dll_delete_node(g->needs_hydrogen->flink);
	}
  } else if (!(dll_empty(g->needs_oh))) {
	m = (struct molecule *) g->needs_oh->flink->val.v;
	m->h2_id = a->id;
	dll_delete_node(g->needs_oh->flink);
	dll_append(g->needs_oxygen, new_jval_v((void *) m));
	pthread_cond_wait(m->h2, g->lock);
  } else {
	m = initialize_m();
	m->h1_id = a->id;
	dll_append(g->needs_oh, new_jval_v((void *) m));
	pthread_cond_wait(m->h1, g->lock);
  }

  hydrogen1 = m->h1_id;
  hydrogen2 = m->h2_id;
  oxygen = m->o_id;
  m->counter++;
  if (m->counter == 3) {
	free(m->h1);
    free(m->h2);
    free(m->o);
    free(m);
  } else {
    pthread_cond_signal(m->o);
    pthread_cond_signal(m->h1);
    pthread_cond_signal(m->h2);
  }

  pthread_mutex_unlock(g->lock);
  rv = Bond(hydrogen1, hydrogen2, oxygen);
  return (void *) rv;
}

void *oxygen(void *arg) {
  struct bonding_arg *a;
  struct global_info *g;
  char *rv;
  struct molecule *m = NULL;
  int hydrogen1, hydrogen2, oxygen;

  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);

  if (!(dll_empty(g->needs_oxygen))) {
	m = (struct molecule *) g->needs_oxygen->flink->val.v;
	m->o_id = a->id;
    dll_delete_node(g->needs_oxygen->flink);
  } else if (!(dll_empty(g->needs_oh))) {
	m = (struct molecule *) g->needs_oh->flink->val.v;
	m->o_id = a->id;
    dll_delete_node(g->needs_oh->flink);
	dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	pthread_cond_wait(m->o, g->lock);
  } else {
	m = initialize_m();
	m->o_id = a->id;
    dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	pthread_cond_wait(m->o, g->lock);
  }

  hydrogen1 = m->h1_id; 
  hydrogen2 = m->h2_id;
  oxygen = m->o_id;
  m->counter++;
  
  if (m->counter == 3) {
	free(m->h1);
	free(m->h2);
	free(m->o);
	free(m);
  } else {
    pthread_cond_signal(m->o);
    pthread_cond_signal(m->h1);
    pthread_cond_signal(m->h2);
  }

  pthread_mutex_unlock(g->lock);
  rv = Bond(hydrogen1, hydrogen2, oxygen);
  return (void *) rv;
}
