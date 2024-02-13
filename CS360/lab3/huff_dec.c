#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
  struct node* zero;
  struct node* one;
  char *data;
} Node;

void free_tree(struct node *head) {
  if (head->zero != NULL) free_tree(head->zero);
  if (head->one != NULL) free_tree(head->one);
  if (head->data != NULL) free(head->data);
  free(head);
}

int main(int argc, char **argv) {

  char *f_name1 = strdup(argv[1]); // code definition file
  char *f_name2 = strdup(argv[2]); // input file
  
  if (argc != 3) {
    fprintf(stderr, "Error: incorrect number of command line arguments.\n");
    exit(1);
  }
   
  int size2 = 0;
  FILE *code_file = fopen(f_name1, "rb");
  FILE *input_file = fopen(f_name2, "rb");

  // create tree from code definition file
  char c;
  Node *root = NULL;
  Node *n = malloc(sizeof(Node));
  n->one = NULL; n->zero = NULL;
  n->data = NULL;
  char *str = malloc(10000); int len;
  str[0] = '\0';
  while(1) {
      c = fgetc(code_file);
      if( feof(code_file) ) {
         break;
      }

	  // set root
	  if (root == NULL) root = n;

      // create string
	  while (c != 0) {
        strncat(str, &c, 1);
        c = fgetc(code_file);
      }
      c = fgetc(code_file);

	  if( feof(code_file) ) {
         break;
      }


	  // create node in tree
	  n = root;
	  while (c == '0' || c == '1') {
		if (c == '1') {
		  if (n->one == NULL) {
            n->one = malloc(sizeof(Node));
            n->one->one = NULL;
            n->one->zero = NULL;
            n->one->data = NULL;
		  }
		  n = n->one;
        }
		if (c == '0') {
		  if (n->zero == NULL) {
            n->zero = malloc(sizeof(Node));
            n->zero->one = NULL;
            n->zero->zero = NULL;
            n->zero->data = NULL;
          }
          n = n->zero;
		}
		c = fgetc(code_file);
	  }

	  // put data in node
	  if (strlen(str) > 0) {
	    n->data = strdup(str);
        len = sizeof(str);
        memset(&str[0], 0, len);
	  }
  }

  fseek(input_file, 0, SEEK_END);
  size2 = ftell(input_file);
  
  // error check size of input file
  if (size2 < 4) {
    fprintf(stderr, "Error: file is not the correct size.\n");
    exit(1);
  }

  // error check for # of bytes
  int t = 0;
  fseek(input_file, -4, SEEK_END);
  fread(&t, 4, 1, input_file);
  fseek(input_file, 0, SEEK_SET);
  int size_check = t/8 + (t%8 != 0) + 4;

  if (size_check != size2) {
    fprintf(stderr, "Error: Total bits = %d, but file's size is %d\n", t, size2);
    exit(1);
  }

  // read in bytes
  int a = 0; unsigned char b;
  unsigned char array[size_check-4];
  while (a < size_check-4) {
    b = fgetc(input_file);
    array[a] = b;
    a++;
  }

  // turn bytes into bits
  char bit = 0; char bit_array[t]; int count = 0;
  for (int i = 0; i < size_check-4; i++) {
    for (int j = 0; j < 8; j++) {
      bit = (array[i] >> j) & 1;
      bit_array[count] = bit;
      count++;
    }
  }
    
  //printf("HI!\n");

  // close files
  fclose(input_file);
  fclose(code_file);

  // decode and print input file
  Node *p = root;
  for (int i = 0; i < t+1; i++) {
    // error check 
    if (p == NULL) {
      fprintf(stderr, "Unrecognized bits\n");
      exit(1);
	}

    if (p->data != NULL) { 
      printf("%s", p->data);
      p = root; 
    }
    bit = bit_array[i];
    if (bit == 0) p = p->zero;
    if (bit == 1) p = p->one;
  }
    
  // free memory
  free(f_name1);
  free(f_name2);
  free_tree(root);
  free(str);
  return 0;
}
