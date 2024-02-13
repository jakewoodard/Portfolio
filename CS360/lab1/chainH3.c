#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* chain_heal initial_range jump_range num_jumps initial_power power_reduction < input_file */

typedef struct Node
{
  char name[100];
  int x, y;
  int cur_PP, max_PP;
  struct Node *prev;
} Node;

int main(int argc, char **argv) {

  // variables
  int initial_range, jump_range, num_jumps, initial_power, arg5;
  double power_reduction;

  // check num of arguments 
  if (argc != 6) {
    printf("Error: incorrect # of arguments\n");
    return 0;
  }

  // sort command line arguments
  initial_range = atoi(argv[1]);
  jump_range = atoi(argv[2]);
  num_jumps = atoi(argv[3]);
  initial_power = atoi(argv[4]);
  arg5 = atof(argv[5]);
  power_reduction = arg5;

  // read in input file & print
  int c = 0; // total # of ppl
  Node *n = malloc(128);
  Node *head = malloc(128);
  Node *temp = malloc(128);
  // Node **NN = NULL;
  char buf[128]; char name[100];
  while (fgets(buf, 128, stdin) != NULL) {
    n = malloc(128);
    sscanf(buf, "%d %d %d %d %[^;]", &n->x, &n->y, &n->cur_PP, &n->max_PP, name);
    printf("%s", name);
    strcpy(n->name, name);
    printf("0x%Lx. \n\n", &n->name);
    if (c == 0) {
      memcpy(head, &n, 128);
      head->prev = NULL;
      memcpy(temp, &n, 128);
    } else {
      memcpy(&n->prev, &temp, 128);
      memcpy(temp, &n, 128);
    }
    c++;
  }
  
  int a = 0;
  while (a < c) {
    printf("0x%Lx.   ", &n->name);
    printf("%s\n", n->name);
    memcpy(n, &n->prev, 128); a++;
  }
  
  // printf("Hi\n");
  return 0;
}
