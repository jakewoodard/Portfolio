// this lab merges two hydrogen atoms with one oxygen atom
// but in reality its just merging threads in this 2-1 ratio
// Lab A bonding
// Kenneth Woodard 
// UTK COSC360

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
  int hydrogen1, hydrogen2, oxygen;
  a = (struct bonding_arg *) arg;
  g = (struct global_info *) a->v;
  pthread_mutex_lock(g->lock);
  //printf("entered hydrogen thread\n");

  if (!(dll_empty(g->needs_hydrogen))) {
	//printf("  needs_hydrogen is not empty\n");
	m = (struct molecule *) g->needs_hydrogen->flink->val.v;
	if (m->h1_id == -1) {
	  m->h1_id = a->id;
	  //printf("h1: %d\n", m->h1_id);
	  dll_delete_node(g->needs_hydrogen->flink);
	  dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	  // h2 not ready yet
	  pthread_cond_wait(m->h1, g->lock);
	} else if (m->h2_id == -1) {
	  m->h2_id = a->id;
	  //printf("h2: %d\n", m->h2_id);
	  dll_delete_node(g->needs_hydrogen->flink);
	  // its ready
	}
  } else if (!(dll_empty(g->needs_oh))) {
	//printf("  needs_oh is not empty\n");
	m = (struct molecule *) g->needs_oh->flink->val.v;
	m->h2_id = a->id;
	//printf("h1: %d\n", m->h1_id);
	//printf("h2: %d\n", m->h2_id);
	dll_delete_node(g->needs_oh->flink); // segfault here
	//printf("fuck this\n", m->h2_id); 
	dll_append(g->needs_oxygen, new_jval_v((void *) m));
	// wait for oxygen now
	pthread_cond_wait(m->h2, g->lock);
  } else {
	//printf("  both lists are empty\n");
	m = initialize_m();
	m->h1_id = a->id;
	//printf("h1: %d\n", m->h1_id);
	dll_append(g->needs_oh, new_jval_v((void *) m));
	pthread_cond_wait(m->h1, g->lock);
  }

  /*if (h2_id == -1) {
	if (o_id == -1) { 
	  dll_append(g->needs_oh, new_jval_v((void *) m));
	  pthread_cond_wait(m->o, g->lock);
	} else {
	  dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	}
	pthread_cond_wait(m->h2, g->lock);
  }*/

  // store into local variables
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

  // unlock mutex and return
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
  //printf("entered oxygen thread\n");

  if (!(dll_empty(g->needs_oxygen))) {
	//printf("  needs_oxygen is not empty\n");
	m = (struct molecule *) g->needs_oxygen->flink->val.v;
	m->o_id = a->id;
    dll_delete_node(g->needs_oxygen->flink);
	//printf("o : %d\n", m->o_id);
	// nothing to wait for...
  } else if (!(dll_empty(g->needs_oh))) {
	//printf("  needs_oh is not empty\n");
	m = (struct molecule *) g->needs_oh->flink->val.v;
	m->o_id = a->id;
    dll_delete_node(g->needs_oh->flink);
	dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	//printf("o : %d\n", m->o_id);
	// wait for hydrogen 2
	pthread_cond_wait(m->o, g->lock);
  } else {
	//printf("  both lists are empty\n");
	m = initialize_m();
	m->o_id = a->id;
    dll_append(g->needs_hydrogen, new_jval_v((void *) m));
	//printf("o : %d\n", m->o_id);
	// wait for hydrogen 1 & 2
	pthread_cond_wait(m->o, g->lock);
  }

  // store into local variables
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
