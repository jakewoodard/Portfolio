/* Will 100% of the class get this question right? */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int i;

  for (i = 0; i < 10; i++) fork();
  printf("Hi\n");
  return 0;
}
