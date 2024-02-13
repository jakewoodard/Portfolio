#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int p1[2], p2[2];
  int in, out;
  FILE *fin, *fout;
  int i, j;

  pipe(p1);
  pipe(p2);

  if (fork() == 0) {
    in = dup(p1[0]);
    out = dup(p2[1]);
  } else {
    in = dup(p2[0]);
    out = dup(p1[1]);
  }
  close(p1[0]); 
  close(p1[1]); 
  close(p2[0]); 
  close(p2[1]); 
  
  fin = fdopen(in, "r");
  fout = fdopen(out, "w");
    
  for (i = 0; i < 10; i++) {
    fprintf(fout, "%d\n", i);
    fflush(fout);
  }

  for (i = 0; i < 10; i++) {
    if (fscanf(fin, "%d", &j) == EOF) return 0;
    printf("j: %d\n", j);
    fflush(stdout);
  }

  return 0;
}
