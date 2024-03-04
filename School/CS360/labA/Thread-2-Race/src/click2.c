#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int i, c;
  int status;

  c = fork();
  if (c != 0) {
    sleep(15);
    return 0;
  }

  sleep(1);

  for (i = 0; i < 5; i++) {
    c = fork();
    if (c == 0) sleep(1); 
  }

  return 0;
}
