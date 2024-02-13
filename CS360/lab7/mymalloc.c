// Kenneth Woodard

#include <stdio.h>
#include <stdlib.h>
// I wanted to #include <unistd.h>
// to silence the warnings, but it wasn't
// allowed according to the lab write up
// so...

typedef struct chunk {
  int size;
  struct chunk *next;
  struct chunk *prev;
} *Chunk;

Chunk malloc_head = NULL;

// helper functions
void *getpage(size_t size);
Chunk bad(Chunk c, size_t size);
void prep(int option, void *p, Chunk t);
void del(Chunk c);
void *spot(size_t size);

void* my_malloc(size_t size) {
  Chunk free_list, tmp;
  void *p;

  // Pad size to multiple of 8
  int num = size % 8;
  if (num != 0) {
    int pad = 8 - num;
	size += pad;
  }	  
  size += 8;
  
  // if first malloc call
  if (malloc_head == NULL) {
	malloc_head = (void *) getpage(size);
    free_list = malloc_head;
	free_list->next = free_list;
	free_list->prev = free_list;
	tmp = bad(free_list, size);
	return (void *) tmp + 8;
  }

  // if not first malloc call
  free_list = (Chunk) spot(size);
  // if a place is found in freelist that
  // fits malloc request
  if (free_list != NULL) {
    tmp = bad(free_list, size);
    return (void *) tmp + 8;
  // if no spot is found for user
  // ask for more memory
  } else {
    p = getpage(size);
	prep(0, p, malloc_head);
    tmp = bad((Chunk) p, size);
    return (void *) tmp + 8;
  }
}

void my_free(void *ptr) {
  Chunk t, tmp;
  ptr -= 8;
  if (malloc_head == NULL) {
    malloc_head = (Chunk) ptr;
    t = (Chunk) ptr;
    t->next = t;
    t->prev = t;
  }
  tmp = malloc_head;
  if (tmp->prev == tmp) {
    if ((void *) tmp < ptr) {
      prep(1, ptr, tmp);
    } else {
      prep(0, ptr, tmp);
      malloc_head = (Chunk) ptr;
    }
    return;
  }
  if (ptr < (void *) malloc_head) {
    prep(0, ptr, tmp);
    malloc_head = (Chunk) ptr;
    return;
  }
  tmp = tmp->prev;
  while (tmp != NULL) {
    if ((void *)tmp < ptr) {
	  if ((void *)tmp->next > ptr || tmp->next == malloc_head) {
    	prep(1, ptr, tmp);
    	return;
      }
    }
	tmp = tmp->prev;
  }
}

void* free_list_begin() {
  return (void *) malloc_head;
}

void* free_list_next(void *node) {
  Chunk tmp = (Chunk) node;
  void *h = free_list_begin();
  if ((void *) tmp->next != h) {
	return (void *) tmp->next;
  }
  return NULL;
}

void coalesce_free_list() {
  Chunk tmp, tmp2;
  for (void *ptr = free_list_begin(); ptr != NULL;) {
    tmp = (Chunk) ptr;
    if (tmp->next != ptr && tmp->next == ptr + tmp->size) {
      tmp2 = (Chunk) tmp->next;
      tmp->size += tmp2->size;
      del(tmp2);
    } else {
      ptr = free_list_next(ptr);
	}
  }
  return;
}

void *getpage(size_t size) {
  void *ptr;
  Chunk tmp;
  if (size > 8184) {
	ptr = (void *) sbrk(size);
	tmp = (Chunk) ptr;
	tmp->size = size;
	return ptr;
  } 
  ptr = (void *) sbrk(8192);
  tmp = (Chunk) ptr;
  tmp->size = 8192;
  return ptr;
}

Chunk bad(Chunk t, size_t s) {
  Chunk tmp;
  if (t->size - s < 16) {
	tmp = t;
	tmp->size = t->size;
	if (t->next == t) {
	  malloc_head = NULL;
	} else {
	  t->next->prev = t->prev;
	  t->prev->next = t->next;
	}
	return tmp;
  }
  tmp = (Chunk) ((void *)t + t->size - s);
  tmp->size = s;
  t->size -= s;
  return tmp;
}

void *spot(size_t size) {
  Chunk poop = (Chunk) free_list_begin();
  if (poop == NULL) {
	perror("Error");
	exit(1);
  }
  while(poop != NULL) {
	if(poop->size >= size) { return (void *) poop; }
	else { poop = (Chunk) free_list_next((void *) poop); }
  }
  return NULL;
}

void prep(int option, void *p, Chunk t) {
  Chunk tmp = (Chunk) p;
  if (option == 0) {
    tmp->next = t;
    tmp->prev = t->prev;
    t->prev->next = p;
    t->prev = p;
  } else {
    tmp->next = t->next;
    tmp->prev = t;
    t->next->prev = p;
    t->next = p;
  }
  return;
}

void del(Chunk node) {
  if (node->next == node) {
    malloc_head = NULL;
    return;
  }
  node->next->prev = node->prev;
  node->prev->next = node->next;
  return;
}
