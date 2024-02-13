#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int a;
  struct node *prev;
} Node;

void display(Node *h, Node *head);

int main() {
  printf("size of node struct = %lu.\n", sizeof(Node));
  
  char buffer[10];  
  int X;
  Node *head, *temp, *p;
  head = NULL; temp =NULL;
  while (fgets(buffer, 10, stdin) != NULL) {
    p = malloc(16);
    p->a = atoi(buffer);
    if(head == NULL) {
      head = p;
      head->prev = NULL;
    } else { 
      p->prev = temp;
    }
    temp = p;
    printf("p = %d  ", p->a);
    printf("addr = 0x%Lx.\n", &p->a);
  }

  printf("head = %d, 0x%Lx.\n", head->a, &head->a);
  printf("Success! Now, traverse.\n");
  display(p, head);
  return 0;
}

// name and address
void display(Node *h, Node *head)
{   
    Node *tmp;
    tmp = h;
    while(tmp != NULL) {   
        printf("%d\n",tmp->a);
        printf("0x%Lx\n", &tmp->a);
        tmp = tmp->prev;
    }
}
