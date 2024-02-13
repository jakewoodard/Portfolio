#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "sockettome.h"
#include "jrb.h"
#include "fields.h"
#include "dllist.h"
#include "jval.h"

JRB Tree; 

typedef struct client {
  int fd;
  char *name;
  FILE *in, *out;
  struct room* r;
} client;

typedef struct room {
  Dllist chats;
  Dllist clients;
  char *name;
  pthread_mutex_t *lock;
  pthread_cond_t *cv;
} room;

void *ch_t(void *v) {
  room *chat;
  client *c;
  Dllist mlist, clientlist;
  chat = (room *) v;
  pthread_mutex_lock(chat->lock);

  while(1) {
    pthread_cond_wait(chat->cv, chat->lock);
    dll_traverse(mlist, chat->chats) {
      if (!dll_empty(chat->clients)) {
        dll_traverse(clientlist, chat->clients) {
          c = (client *) clientlist->val.v;
          if (fputs(mlist->val.s, c->out) == EOF) {
            dll_delete_node(clientlist);
            fclose(c->out);
          }
          if (fflush(c->out) == EOF) {
            dll_delete_node(clientlist);
            fclose(c->out);
          }
        }
      }
    }
    free_dllist(chat->chats);
    chat->chats = new_dllist();
  }
  return NULL;
}

void *client_t(void *v) {
  char comm[1000], buf[1000];
  client *c, *tmpc;
  room *chat;
  Dllist tmpl, rm;
  JRB tmp;
  c = (void *)v;
  c->out = fdopen(c->fd, "w"); 
  if (c->out == NULL) { 
    perror("making fout");
    exit(1); 
  }
  c->in = fdopen(c->fd, "r"); 
  if (c->in == NULL) { 
    perror("making fin"); 
    exit(-1);
  }
  if (fputs("Chat Rooms:\n\n", c->out) == EOF) {
    fclose(c->out); fclose(c->in);
    pthread_exit(NULL);
  }
  if (fflush(c->out) == EOF) {
    fclose(c->out); fclose(c->in);
    pthread_exit(NULL);
  }

  jrb_traverse(tmp, Tree) {
    chat = (room *) tmp->val.v;
    if (fputs(chat->name, c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
    if (fflush(c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
    if (fputs(":", c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
    if (fflush(c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
	dll_traverse(tmpl, chat->clients) {
      if (fputs(" ", c->out) == EOF) {
        fclose(c->out); fclose(c->in);
        pthread_exit(NULL);
      }
      if (fflush(c->out) == EOF) {
        fclose(c->out); fclose(c->in);
        pthread_exit(NULL);
      }
      tmpc = (client *) tmpl->val.v;
      if (fputs(tmpc->name, c->out) == EOF) {
        fclose(c->out); fclose(c->in);
        pthread_exit(NULL);
      }
      if (fflush(c->out) == EOF) {
        fclose(c->out); fclose(c->in);
        pthread_exit(NULL);
      }
    }
    if (fputs("\n", c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
      fflush(c->out);
    }
    if (fputs("\n", c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
    if (fflush(c->out) == EOF) {     
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);
    }
    if (fputs("Enter your chat name (no spaces):\n", c->out) == EOF) {
	  fclose(c->out); fclose(c->in);
      pthread_exit(NULL);   
    }
    if (fflush(c->out) == EOF) {
      fclose(c->out); fclose(c->in);
      pthread_exit(NULL);     
    }
    if (fgets(comm, sizeof(comm), c->in) == NULL) {   
      fclose(c->out); fclose(c->in);
	  pthread_exit(NULL);
    }  

    comm[strlen(comm)-1] = '\0';                            
    printf("Client name entered: %s\n", comm); 
    c->name = strdup(comm);    

    if (fputs("Enter chat room:\n", c->out) == EOF) {
      fclose(c->in); fclose(c->out);
	  pthread_exit(NULL);   
    } 
    if (fflush(c->out) == EOF) {
      fclose(c->out); fclose(c->in);
	  pthread_exit(NULL);
    } 
    if (fgets(comm, sizeof(comm), c->in) == NULL) {
      fclose(c->in); fclose(c->out);
	  pthread_exit(NULL);
    }
    comm[strlen(comm)-1] = '\0';
    printf("Room name entered: %s\n", comm);
    tmp = jrb_find_str(Tree, comm); 

    if (tmp != NULL) {
      chat = (room *) tmp->val.v;
      c->r = chat;
      pthread_mutex_lock(chat->lock);
      dll_append(chat->clients, new_jval_v((void *) c));
	  strcpy(comm, c->name);
	  strcat(comm, " has joined\n\0");
	  dll_append(chat->chats, new_jval_s(strdup(comm)));
	  pthread_cond_signal(chat->cv);
	  pthread_mutex_unlock(chat->lock);
    } else { fprintf(stderr, "Chatroom %s does not exist\n", chat->name); exit(1); }

    while (1) {
	    strcpy(buf, c->name);
	    strcat(buf, ": ");
        if (fgets(comm, sizeof(comm), c->in) == NULL) {
          fclose(c->in);
		  strcpy(buf, c->name);
		  strcat(buf, " has left\n");
		  pthread_mutex_lock(chat->lock);
		  dll_append(chat->chats, new_jval_s(strdup(buf)));
          if (c->out != 0) { 
              fclose(c->out);
              rm = NULL;
              dll_traverse(tmpl, chat->clients) {
                tmpc = (client *) tmpl->val.v;
				if (strcmp(c->name, tmpc->name) == 0) rm = tmpl;
              }
              if (rm != NULL) dll_delete_node(rm);
          } 
		  pthread_cond_signal(chat->cv);  
		  pthread_mutex_unlock(chat->lock);
		  pthread_exit(NULL);
        } else {
          strcat(buf, comm);
		  pthread_mutex_lock(chat->lock);
		  dll_append(chat->chats, new_jval_s(strdup(buf)));
		  pthread_cond_signal(chat->cv);
		  pthread_mutex_unlock(chat->lock);
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
  int D, sock;
  client *c;
  room *chatter;
  pthread_t *tid;
  int L = 0;
  printf("Initialize Server\n");
  if (argc < 3) {
    fprintf(stderr, "usage: chat_server host port [chatrooms]\n");
    exit(1);
  }
  
  Tree = make_jrb();
  for (int i = 2; i < argc; i++) {
    chatter = (room *) malloc(sizeof(room));
    chatter->clients = new_dllist();
    chatter->chats = new_dllist();
    chatter->lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(chatter->lock, NULL);
    chatter->cv = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(chatter->cv, NULL);
    chatter->name = (char *) malloc(sizeof(char)*strlen(argv[i]));
    chatter->name = strdup(argv[i]); 
    tid = (pthread_t *) malloc(sizeof(pthread_t));
    if (pthread_create(tid, NULL, ch_t, chatter) != 0) {
      perror("pthread_create");
	  exit(1);
    }  
	pthread_detach(*tid);
    jrb_insert_str(Tree, chatter->name, new_jval_v((void *)chatter));
  }

  L = atoi(argv[1]);
  printf("Chatrooms made\n");   
  sock = serve_socket(L); 

  while (1) {
    D = accept_connection(sock);
    c = (client *) malloc(sizeof(client));
    c->name = NULL;
    c->r = NULL;
    c->in = NULL;
    c->out = NULL;
	c->fd = D;
    tid = (pthread_t *) malloc(sizeof(pthread_t));
    if (pthread_create(tid, NULL, client_t, c) != 0) {
      perror("pthread_create");
      exit(1);
    }
    printf("Added client\n");
    pthread_detach(*tid);
  }
  return 0;
}
