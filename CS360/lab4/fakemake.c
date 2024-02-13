#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

typedef struct Make {
  Dllist c;
  Dllist h;
  Dllist lib;
  Dllist f;
  Dllist o;
  char *name;
} Make;

void FreeMem(Make *m);
void ReadFile(IS is, Make *m);
void ReadList(IS is, Dllist list);
void ClearList(Dllist list);
void MakeSystemCall(Make *m, char *file);
int NowH(Make *m, Dllist list);
int NowC(Make *m, int time);

int main(int argc, char **argv) {

  FILE *input_file;
  IS is;
  int time;
  Make *m;

  // error check first
  if (argc == 1) {
	is = new_inputstruct("fmakefile");
    if (is == NULL) {
		fprintf(stderr, "Error\n");
		exit(1);
	}
  }
  if (argc == 2) {
	is = new_inputstruct(argv[1]);
	if (is == NULL) {
	  fprintf(stderr, "Error\n");
	  exit(1);
	}
  }

  // allocate mem for m (make struct)
  m = malloc(sizeof(Make));
  m->c = new_dllist();
  m->h = new_dllist();
  m->f = new_dllist();
  m->o = new_dllist();
  m->lib = new_dllist();
  ReadFile(is, m);

  if (m->h != NULL) time = NowH(m, m->h);

  int c = NowC(m, time);

  if (c == 0) {
    printf("%s up to date\n", m->name);
    FreeMem(m);
    jettison_inputstruct(is);
	exit(1);
  }

  // free allocated memory
  FreeMem(m);
  jettison_inputstruct(is);
  return 0;
}

// free memory allocated
void FreeMem(Make *m) {
	if (m->c != NULL) ClearList(m->c);
	if (m->h != NULL) ClearList(m->h);
	if (m->f != NULL) ClearList(m->f);
	if (m->o != NULL) ClearList(m->o);
	if (m->lib != NULL) ClearList(m->lib);
	free(m->name);
	free(m);
	return;
}

// read info
void ReadFile(IS is, Make *m) {
  int i = 0;
  while (get_line(is) >= 0) {
	if (is->NF != 0) {
	  if ((*is->fields[0]) == 'C') { ReadList(is, m->c); }
	  else if ((*is->fields[0]) == 'H') { ReadList(is, m->h); }
	  else if ((*is->fields[0]) == 'F') { ReadList(is, m->f); }
	  else if ((*is->fields[0]) == 'L') { ReadList(is, m->lib); }
	  else if ((*is->fields[0]) == 'E') {
        i++;
		if (i > 1) {
		  fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
		  exit(1);
		}
		m->name = strdup(is->fields[1]);
	  }
	}
  }
  if (i != 1) {
    fprintf(stderr, "No executable specified\n");
    exit(1);
  }
  return;
}

// go through & fill list
void ReadList(IS is, Dllist list) {
  for(int i = 1; i < is->NF; i++){
	dll_append(list, new_jval_s(strdup(is->fields[i])));
  }
  return;
}

// traverse & free list
void ClearList(Dllist list) {
  Dllist t;
  dll_traverse(t, list) free(t->val.s);
  free_dllist(list);
  return;
}

void MakeSystemCall(Make *m, char *file) {
  Dllist t; int a = 0;
  char *str = malloc(512);
  strcat(str, "gcc -c ");
  
  dll_traverse(t, m->f) {
	strcat(str, t->val.s);
	strcat(str, " ");
  }
  strcat(str, file);
  printf("%s\n", str);
  a = system(str);

  if (a != 0) {
	fprintf(stderr, "Command failed.  Exiting\n");
	exit(1);
  }
  free(str);
  return;
}

int NowH(Make *m, Dllist list) {
  Dllist t;
  struct stat s;
  int i = 0;
  dll_traverse(t, list) {
    if(stat(t->val.s, &s) == 0) {
	  int time = s.st_mtime;
	  if(time > i) i = time;
	} else {
	  fprintf(stderr, "fmakefile: %s: No such file or directory\n", t->val.s);
	  exit(-1);
	}
  }
  return i;
}

int NowC(Make *m, int time) {
  Dllist t; 
  struct stat s;
  struct stat tmp;
  int i = 0; int call;
  char *ofile = (char *) malloc(1024);
  int c = 0;

  dll_traverse(t, m->c){
	if (stat(t->val.s, &s) == 0) {    
	  ofile[0] = '\0';
	  strncat(ofile, t->val.s, (strlen(t->val.s) - 1));
	  strcat(ofile, "o");
	  dll_append(m->o, new_jval_s(strdup(ofile)));	

	  if (stat(ofile, &tmp) == 0) {
		if (tmp.st_mtime < time) { 					
		  c++;
		  MakeSystemCall(m, t->val.s);
		} else if (tmp.st_mtime < s.st_mtime) {
		  c++;
		  MakeSystemCall(m, t->val.s);
		} else { }
	  }	else {
		c++;
		MakeSystemCall(m, t->val.s);
	  }
	} else {
	  fprintf(stderr, "fmakefile: %s: No such file or directory\n", t->val.s);
	  exit(1);
	}
  }
  free(ofile);
  
  if (stat(m->name, &s) == 0) {
	int max = NowH(m, m->o);
	if (max > s.st_mtime) c++;
  } else { c++; }
  if (c != 0) {
	char *str = malloc(1024);
	str[0] = '\0';
	strcat(str, "gcc -o ");
	strcat(str, m->name);
	strcat(str, " ");

	dll_traverse(t, m->f) {
	  strcat(str, t->val.s);
	  strcat(str, " ");
	}
	dll_traverse(t, m->o) {
	  strcat(str, t->val.s);
	  strcat(str, " ");
	}
	if (m->lib != NULL) { 
	  dll_traverse(t, m->lib) {
		strcat(str, t->val.s);
		strcat(str, " ");
	  }
	}
	str[strlen(str) - 1] = '\0';
	printf("%s\n", str);

	call = system(str);
	if (system(str) < 0) {
	  fprintf(stderr, "Command failed.  Fakemake exiting\n");
	  exit(1);
	}
	free(str);
  }
  return c;
}
