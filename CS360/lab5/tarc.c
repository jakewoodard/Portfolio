#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "jrb.h"
#include "dllist.h"

int compare(Jval v1, Jval v2)
{
  if (v1.l < v2.l) return -1;
  if (v1.l > v2.l) return 1;
  return 0;
}

int tar_time(const char *fn, JRB inodes, int check, int stop) 
{
  DIR *d;                   /* Return value of opendir(). */
  struct dirent *de;        /* Return value of each readdir() call. */
  struct stat buf;          /* The information about each file returned by stat() */
  int exists;               /* Return value of stat on each file. */
  long total_size;          /* The total size of all files. */
  int fn_size;              /* This is the length of fn -- so we can build the filename. */
  char *dir_fn;             /* This will be the filename including the directory. */
  int dir_fn_size;          /* This is the bytes in dir_fn is, in case we need to make it bigger. */
  int sz;
  FILE *fp;
                // for printing...
  char *data;   // file data
  int i, m;     // name size, mode
  long n, t, s; // inode, time, size
  char *suffix; // name for printing

  Dllist directories, tmp;  /* Dllist of directory names, for doing recusion after closing. */

  /* Initialize */
  d = opendir(fn);
  if (d == NULL) {
    perror(fn);
    exit(1);
  }
  directories = new_dllist();

  if (check == 0) {
    // find last slash character before directory name
    for (int i = 0; i < strlen(fn); i++) {
	  if (fn[i] == 0x2f) stop = i;
    }
	if (stop > 0) stop++;
    // remove prefix
    int di = strlen(fn) - stop;
    suffix = malloc(di);	
    for (int i = 0; i < di; i++) {
	  suffix[i] = fn[i+stop];
    }

    // print info to stdout for main directory
	lstat(fn, &buf);
	i = strlen(suffix);
	fwrite(&i, 4, 1, stdout);
    fwrite(suffix, i, 1, stdout);
    n = buf.st_ino;
	fwrite(&n, 8, 1, stdout);
	m = buf.st_mode;
	fwrite(&m, 4, 1, stdout);
	t = buf.st_mtime;
	fwrite(&t, 8, 1, stdout);
	check++;
	free(suffix);
  }

  /* Start building the directory + files.   We'll start by setting dir_fn_size to fn_size+10,
     and we'll make it bigger as we need to.  It will be more efficient to use a number bigger
     than 10 for this, but 10 will let us debug the code if there's a problem.

     I'm also setting up dir_fn to hold the directory name and a slash. */
  fn_size = strlen(fn);
  dir_fn_size = fn_size + 100;
  dir_fn = (char *) malloc(sizeof(char) * dir_fn_size);
  if (dir_fn == NULL) { perror("malloc dir_fn"); exit(1); }
  strcpy(dir_fn, fn);
  strcat(dir_fn + fn_size, "/");

  /* Run through the directory and run stat() on each file  */
  for (de = readdir(d); de != NULL; de = readdir(d)) {

    /* First, we need to build dir_fn.  First check to see if it's big enough, and if not,
       we'll call realloc() to reallocate space.  Then we put the filename after the
       slash. */
    if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
      sz = strlen(de->d_name);
      if (dir_fn_size < fn_size + sz + 2) {    
		dir_fn_size = fn_size + sz + 10;
        dir_fn = realloc(dir_fn, dir_fn_size);
      }
      strcpy(dir_fn + fn_size + 1, de->d_name);

	  // stat and error check
      exists = lstat(dir_fn, &buf);
      if (exists < 0) {
        fprintf(stderr, "Couldn't stat %s\n", dir_fn);
        exit(1);
      }

	  // remove prefix
      int di2 = strlen(dir_fn) - stop;
      suffix = malloc(di2);
      for (int i = 0; i < di2; i++) {
        suffix[i] = dir_fn[i+stop];
      }

      // print file size & name & inode
      i = strlen(suffix);
      fwrite(&di2, 4, 1, stdout);
	  printf("%.*s", di2, suffix);
	  n = buf.st_ino;
      fwrite(&n, 8, 1, stdout);
      free(suffix);
    }

	/* Check the inodes tree to check if we've seen this file before.
       If so, ignore.  If not, then print file mod time and mode to stdout. */
    if (jrb_find_gen(inodes, new_jval_l(buf.st_ino), compare) == NULL) {
      jrb_insert_gen(inodes, new_jval_l(buf.st_ino), new_jval_i(0), compare);
	  if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
		m = buf.st_mode;
		fwrite(&m, 4, 1, stdout);
		t = buf.st_mtime;
		fwrite(&t, 8, 1, stdout);
		// if file is not a directory print size and bytes of file
        if (S_ISREG(buf.st_mode)) {
          s = buf.st_size;
          fwrite(&s, 8, 1, stdout);
          fp = fopen(dir_fn, "rb");
          if (fp == NULL) { perror(dir_fn); exit(1); }
          data = (char *) malloc(buf.st_size);
		  int p = fread(data, 1, buf.st_size, fp);
          fwrite(data, p, 1, stdout);
		  fclose(fp);
          free(data);
        }
	  }
    }

    /* Don't make the recursive call, but instead put the directory into the dllist. */
    if (S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
      dll_append(directories, new_jval_s(strdup(dir_fn)));
    }
  }

  /* Make the recursive calls after you've closed the directory.  */
  closedir(d);
  dll_traverse(tmp, directories) {
    tar_time(tmp->val.s, inodes, check, stop);
  }

  /* Clean up.  You need to free the strings inside the dllist, because you
     allocated them with strdup(), and they'll be a memory leak otherwise. */
  dll_traverse(tmp, directories) free(tmp->val.s);
  free_dllist(directories);
  free(dir_fn);

  return 0;
}

int main(int argc, char **argv)
{
  long total_size;
  JRB inodes;
  
  inodes = make_jrb();
  tar_time(argv[1], inodes, 0, 0);
  return 0;
}
