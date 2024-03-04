#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* chain_heal initial_range jump_range num_jumps initial_power power_reduction < input_file */

typedef struct node
{
  char name[104];
  int x, y;
  int cur_PP, max_PP;
  int adj_size;
  struct node **adj;
  int visited, healing;
} Node;

typedef struct input_info {
  int initial_range;
  int jump_range;
  int num_jumps;
  int initial_power;
  double power_reduction;
  int best_path_length;
  Node **best_path;
  int *healing;
} Info;

int best_heal;
Node *head;

void DFS(Node *n, Info I, int hop, int total_healing);

int main(int argc, char **argv) {
    
  // variables
  int initial_range, jump_range, num_jumps, initial_power;
  double power_reduction;

  // check num of arguments 
  if (argc != 6) {
    printf("Error: incorrect # of arguments\n");
    return 0;
  }
  
  int best_heal = 0;
  // sort command line arguments
  initial_range = atoi(argv[1]);
  jump_range = atoi(argv[2]);
  num_jumps = atoi(argv[3]);
  initial_power = atoi(argv[4]);
  power_reduction = atof(argv[5]);

  Info I;  
  I.initial_range = initial_range;
  I.jump_range = jump_range;
  I.num_jumps = num_jumps;
  I.initial_power = initial_power;
  I.power_reduction = power_reduction;
  best_heal = 0;

  // read in input file & print
  int c = 0; // total # of ppl
  Node *n;
  head = NULL;
  char buf[128]; char name[104];
  printf("size of *node = %d\n", sizeof(Node));

  while (fgets(buf, 128, stdin) != NULL) {
    n = malloc(sizeof(Node));
    sscanf(buf, "%d %d %d %d %[^;]", &n->x, &n->y, &n->cur_PP, &n->max_PP, name);
    strcpy(n->name, name);
    n->adj_size = 0;
    n->visited = 0;
    n->healing = 0;
    c++;
  }

  // store the nodes here
  Node *NN[c];  int a = 0;
  while (a < c) {
    NN[a] = n;
    NN[a]->adj = malloc(8);
    printf("0x%Lx.    ", &NN[a]->name);
    printf("%s", NN[a]->name);
    n--; a++;
  }
  a = 0;

  // set up adjaceny list
  int b = 0;
  double D = 0; // distance
  while(a < c) {
    printf("\n");
    printf("adjacency list for ");
    printf("%s", NN[a]->name);
    b = 0;
    while(b < c) {
      if (a != b) {
        D = sqrt(pow((NN[a]->x - NN[b]->x), 2) + pow((NN[a]->y - NN[b]->y), 2));
        if (D <= jump_range) {
          //temp[a] = NN[b];
          NN[a]->adj[NN[a]->adj_size] = NN[b];
          NN[a]->adj_size++;
          printf("%s", NN[b]->name);
        }
      }
      b++;
    }
    a++;
  }
  a = 0;
  head = NN[c-1];
  

  int total_healing = 0;
  // depth first search on all nodes within init_range
  printf("\n\n"); printf("DFS time\n"); 
  for (b = c-1; b >= 0 ; b--) {
    D = sqrt(pow((NN[c-1]->x - NN[b]->x), 2) + pow((NN[c-1]->y - NN[b]->y), 2));
    // convert I to double
    if (D <= I.initial_range) {
      DFS(NN[b], I, 1, total_healing);
    }
  }

  // print healing info  
  printf("TOTAL HEALING: %d\n", best_heal);

  // free mem allocated
  a = 0;
  while (a < c) { 
    free(NN[a]->adj);
    free(NN[a]);
    a++;
  }
  free(n);

  printf("Success!\n");
  return 0;
}

void DFS(Node *n, Info I, int hop, int total_healing) {
  int i;

  if (hop > I.num_jumps)
    return;

  printf("hop:%d", hop);
  printf("  %s", n->name);
  n->visited = 1;
  
  if (hop != 1) I.initial_power = rint((1 - I.power_reduction) * I.initial_power);
  // healing calculation
  if (n->max_PP - n->cur_PP <= I.initial_power) {
    n->healing = n->max_PP - n->cur_PP;
  } else {
    n->healing = I.initial_power;
  }
  total_healing += n->healing;

  if (total_healing > best_heal) best_heal = total_healing;
 
  for (i = 0; i < n->adj_size; i++) {
    if (n->adj[i]->visited == 0) {
      DFS(n->adj[i], I, hop+1, total_healing);
    }
  }
  n->visited = 0;
}
