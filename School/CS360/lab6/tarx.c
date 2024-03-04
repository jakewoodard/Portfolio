#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <sys/types.h>
#include "jrb.h"
#include "dllist.h"
#include "fields.h"
#include "jval.h"

typedef struct {
  char *name;
  char *data;
  char *link;
  long inode;
  long time;
  long size;
  int mode;
} filenode;

int main()
{
  FILE *fp = stdin; 
  FILE *fout = NULL;
  int fn_size;
  char *fn;             
  JRB inodes = make_jrb();
  Dllist list = new_dllist();
  Dllist tmp;
  filenode *F, *F2;

  // variable for time
  struct utimbuf new_time = {};

  int fn_size_check, inode_check;
  // iterate through tar file
  while (fn_size_check = fread(&fn_size, 1, 4, fp)) {

	if (fn_size_check != 4) {
	  perror(" ");
	  exit(1);
	}

	F = malloc(sizeof(filenode));
	fn = malloc(fn_size + 1);
	fread(fn, fn_size, 1, fp);
	fn[fn_size] = '\0';

	inode_check = fread(&F->inode, 8, 1, fp);
	if (inode_check != 1) {
	  perror(" ");
      exit(1);
    }

	F->name = strdup(fn);
	F->time = 0; F->size = 0; F->mode = 0; 
	F->data = NULL; F->link = NULL;

	// have we seen this inode before?
	if (jrb_find_int(inodes, F->inode) == NULL) {

	  // read in mode, modification time
	  if (fread(&F->mode, 4, 1, fp) != 1) {
		perror(F->name);
		exit(1);
	  }
	  if (fread(&F->time, 8, 1, fp) != 1) {
		perror(F->name);
        exit(1);
	  }

	  // read normal file data
	  if (!S_ISDIR(F->mode)) {
		if (fread(&F->size, 8, 1, fp) != 1) {
		  perror(F->name);
          exit(1);
		}
		if (F->size < 0 || F->size > 1000) {
		  perror(fn);
		  exit(1);
		}
        F->data = malloc(F->size);
        if (fread(F->data, F->size, 1, fp) != 1) {
		  perror(F->name);
		  exit(1);
		}
	  }
	  // insert file into jrb
      jrb_insert_int(inodes, F->inode, new_jval_v((void *)F));

	} else {
	  F2 = (filenode *) jrb_find_int(inodes, F->inode)->val.v;
	  F->link = strdup(F2->name);
	  F->mode = F2->mode;
	  F->time = F2->time;
	  if (!S_ISDIR(F->mode)) {
		F->size = F2->size;
		F->data = strdup(F2->data);
	  }
	}
	dll_append(list, new_jval_v((void *)F));
	free(fn);
  }
  fclose(fp);

  // printf("done reading in\n");

  // traverse list of files and recreate directory
  // as well as linking files together
  int c = 0;
  dll_traverse(tmp, list) {
	F = (filenode *) tmp->val.v;
    //	printf("%d, %s\n", ++c, F->name);
	if (S_ISREG(F->mode)) {
	  if (F->link == NULL) {
		fout = fopen(F->name, "w");
        if (fout == NULL) {
          perror(F->name);
          exit(1);
        }
		fwrite(F->data, F->size, 1, fout);
		fclose(fout);
	  } else {
		link(F->link, F->name);
		free(F->link);
	  }
	} else { 
	  mkdir(F->name, 0777);
	}
  }

  //printf("directory created\n");

  // now must update modes and modification
  // times for all files as well as freeing 
  // filenames and filenode pointers
  dll_rtraverse(tmp, list) {
	F = (filenode *) tmp->val.v;

	// mode first
	chmod(F->name, F->mode);

	// time second
	new_time.modtime = F->time;
	utime(F->name, &new_time);

	// free third
	if (F->name != NULL) free(F->name);
	if (F->data != NULL) free(F->data);
    free(F);
  }

  jrb_free_tree(inodes);
  free_dllist(list);
  return 0;
}
