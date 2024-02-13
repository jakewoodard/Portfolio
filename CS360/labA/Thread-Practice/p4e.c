#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *printme(void *ip)
{
  int *i;

  i = (int *) ip;
  printf("Hi.  I'm thread %d\n", *i);
  pthread_exit(NULL);
  return NULL;
}

int main()
{
  int i;                  /* I've deleted the ids array, and we'll just pass &i */
  pthread_t tids[4];
  void *retval;

  for (i = 0; i < 4; i++) {
    if (pthread_create(tids+i, NULL, printme, &i) != 0) {   // Here's the change.
      perror("pthread_create");
      exit(0);
    }
  }

  pthread_exit(NULL);

  return 0;
}
