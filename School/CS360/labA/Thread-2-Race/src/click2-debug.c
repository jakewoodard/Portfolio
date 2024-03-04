#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int i, c;
  int status;
  char line[100];

  c = fork();
  if (c != 0) {

    /* The original parent prints out the process id of its child. */

    printf("The first child process is %d\n", c);
    fflush(stdout);

    /* Now it sleeps.  When it wakes up, it calls "ps x" to print out its child's status.
       The child should be a zombie process. */

    sleep(15);
    printf("Original parent process is exiting.  Child is a zombie:\n");
    sprintf(line, "ps x | grep %d | sed /grep/d", c); 
    system(line);

    /* The original process exits */

    return 0;
  }

  sleep(1);

  /* This loop creates 31 more processes. */

  for (i = 0; i < 5; i++) {
    c = fork();
    if (c == 0) {
      sleep(1); 
    } else {

      /* Have the parent print the process id of its child. */

      printf("Process %d just created child %d\n", getpid(), c);
      fflush(stdout);
    }
  }

  /* When each process exits, have it print out its process id and its parent's.
     For all processes but one, the parent will be 1, as the process is an orphan. */

  printf("Process %d exiting -- parent is %d\n", getpid(), getppid());
  fflush(stdout);

  return 0;
}
