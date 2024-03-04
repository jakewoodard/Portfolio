#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
  struct node* zero;
  struct node* one;
  char *data;
} Node;

void display(struct node *head) {

  if (head->zero != NULL) {
    display(head->zero);
  }
  if (head->one != NULL) {
    display(head->one);
  }
  if (head->data != NULL) {
    printf("node: %s\n", head->data);
  }
  return;
}

void free_tree(struct node *head) {
  if (head->zero != NULL) {
    free_tree(head->zero);
  }
  if (head->one != NULL) {
    free_tree(head->one);
  }
  free(head);
}

int main(int argc, char **argv) {

  char *f_name1 = strdup(argv[1]); // code definition file
  char *f_name2 = strdup(argv[2]); // input file
  
  if (argc != 3) {
    fprintf(stderr, "Error: incorrect number of command line arguments.\n");
    exit(1);
  }
  
  int size1 = 0; int size2 = 0;
  FILE *code_file = fopen(f_name1, "rb");
  FILE *input_file = fopen(f_name2, "rb");
  char buffer1[8192];
  char buffer2[16384];

  // create tree from code definition file
  char c; char prev_c;
  Node *root = NULL;
  Node *n = malloc(sizeof(Node));
  n->one = NULL; n->zero = NULL;
  n->data = NULL;
  char *str = malloc(64);
  while(1) {
      c = fgetc(code_file);
      if( feof(code_file) ) {
         break;
      }

      if (root == NULL) root = n;

      if (c == 49) {
        if (n->one == NULL) {
          n->one = malloc(sizeof(Node));
          n->one->one = NULL;
          n->one->zero = NULL;
          n->one->data = NULL;
        }
        n = n->one;
      } else if (c == 48) {
        if (n->zero == NULL) {
          n->zero = malloc(sizeof(Node));
          n->zero->one = NULL;
          n->zero->zero = NULL;
          n->zero->data = NULL;
        }
        n = n->zero;
      } else if (c == 0 && prev_c != '0' && prev_c != '1') {
        n = root;
      } else if (c != 0 && c != 48 && c != 49) {
        strncat(str, &c, 1);
      } else {
        if (strlen(str) > 0 && prev_c != 0) {
          n->data = strdup(str);
          memset(&str[0], 0, sizeof(str));
        }
      }
      prev_c = c;
  }

  fseek(code_file, 0, SEEK_SET);
  size1 = fread(buffer1, 1, 8192, code_file);
  printf("The size of code definition file is: %d\n", size1);
  size2 = fread(buffer2, 1, 16384, input_file);
  
  // error check size of input file
  if (size2 < 4) {
    fprintf(stderr, "Error: Input file size too small.\n");
    exit(1);
  }
  printf("The size of input file is: %d\n", size2);

  // error check for # of bytes
  int t = 0; char *tt = malloc(4);
  fseek(input_file, -4, SEEK_END);
  fgets(tt, 4, input_file);
  t = ((unsigned char)(tt[3]) << 24 |
          (unsigned char)(tt[2]) << 16 |
          (unsigned char)(tt[1]) << 8 |
          (unsigned char)(tt[0]));
  fseek(input_file, 0, SEEK_SET);
  printf("# of bits in input file = %d\n\n", t);
  int size_check = t/8 + (t%8 != 0) + 4;
  if (size_check != size2) {
     fprintf(stderr, "Error: input file is wrong size\n");
     exit(1);
  }

  // read in bytes
  printf("Bytes: ");
  int a = 0; unsigned char b;
  unsigned char array[size_check-4];
  while (a < size_check-4) {
    b = fgetc(input_file);
    array[a] = b;
    printf("%c", array[a]);
    a++;
  }
  printf("\nNow bits: ");
  // turn bytes into bits
  int bit = 0; int bit_array[t]; int count = 0;
  for (int i = 0; i < size_check-4; i++) {
    for (int j = 0; j < 8; j++) {
      bit = (array[i] >> j) & 1;
      bit_array[count] = bit;
      printf("%d", bit);
      count++;
    }
  }
  printf("\n\n");

  // close files
  fclose(input_file);
  fclose(code_file);

  // display tree
  printf("traverse & display tree\n");
  display(root);

  // decode and print input file
  printf("Decoded input string: \n");
  Node *p = root;
  for (int i = 0; i < t+1; i++) {
    // error check 
    if (p == NULL) {
      fprintf(stderr, "Error: can't decode input file\n");
    }
    if (p->data != NULL) { 
      printf("%s", p->data);
      p = root; 
    }
    bit = bit_array[i];
    if (bit == 0) p = p->zero;
    if (bit == 1) p = p->one;
  }
  printf("\n");
  
  // free memory
  free(f_name1);
  free(f_name2);
  free_tree(root);
  free(str);
  return 0;
}
