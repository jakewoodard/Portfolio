#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

typedef struct {
	char *Sex;
	char *Name;
	void *Father;
	void *Mother;
	int visited;
	Dllist Children;
} Person;

char *get_name(IS is);
Person *new_person(char *name, JRB people);
void add_child(Person *p, Dllist children);
int is_descendant(Person *p);
void print_order(Person *x);
void print_fam(Person *x);

int main(int argc, char **argv) {

	JRB people, t, tmp;
	IS is;
    Person *p, *x;
	Person *j; 
	char *name;
	
	// make rb tree
	people = make_jrb();
    
	// open file
	is = new_inputstruct(NULL);
    
	// error check file
	if (is == NULL) {
		perror("Error\n");
		exit(1);
	}

	// read file
    while(get_line(is) >= 0) {

	  // if line is blank continue to next
	  if (is->NF > 0) {
	    
		// check for person
        if (strcmp("PERSON", is->fields[0]) == 0) {
          name = strdup(get_name(is));
		  tmp = jrb_find_str(people, name);
		  if (tmp == NULL) {
		    p = new_person(name, people);
		  } else { 
			p = (Person *) tmp->val.v; 
		  }
		} else if (strcmp("SEX", is->fields[0]) == 0) {
		  if (strcmp("F", is->fields[1]) == 0) {
			p->Sex = "Female";
		  } else if (strcmp("M", is->fields[1]) == 0) {
			p->Sex = "Male";
		  } else {
			p->Sex = NULL;
		  }
		} else if (strcmp("FATHER_OF", is->fields[0]) == 0) {
	      name = strdup(get_name(is));
		  tmp = jrb_find_str(people, name);
		  if (tmp == NULL) {
            j = new_person(name, people);
          } else {
            j = (Person *) tmp->val.v;
          }
		  j->Father = p;
		  add_child(j, p->Children);
		  p->Sex = "Male";
		} else if (strcmp("MOTHER_OF", is->fields[0]) == 0) {
		  name = strdup(get_name(is));
          tmp = jrb_find_str(people, name);
          if (tmp == NULL) {
            j = new_person(name, people);
          } else {
            j = (Person *) tmp->val.v;
          }
          j->Mother = p;
          add_child(j, p->Children);
          p->Sex = "Female";
		} else if (strcmp("FATHER", is->fields[0]) == 0) {
          name = strdup(get_name(is));
          tmp = jrb_find_str(people, name);
          if (tmp == NULL) {
            j = new_person(name, people);
          } else {
            j = (Person *) tmp->val.v;
          }
          p->Father = j;
          j->Sex = "Male";
          add_child(p, j->Children);
		} else if (strcmp("MOTHER", is->fields[0]) == 0) {
		  name = strdup(get_name(is));
          tmp = jrb_find_str(people, name);
		  if (tmp == NULL) {
			j = new_person(name, people);
          } else {
            j = (Person *) tmp->val.v;
          }
		  p->Mother = j;
		  j->Sex = "Female";
		  add_child(p, j->Children);
		}
	  }
	}

    // test for cycles
	jrb_traverse(t, people) {
        x = (Person *) t->val.v;
		if (is_descendant(x)) {
            fprintf(stderr, "Bad input -- cycle in specification\n");
            exit(1);
        }
	}

    // set all visited values back to zero
	jrb_traverse(t, people) {
		x = (Person *) t->val.v;
		x->visited = 0;
	}
	 
    // print 
	jrb_traverse(t, people) {
        x = (Person *) t->val.v;
		print_order(x);
    }
	
	// free memory
	jrb_traverse(t, people) {
        x = (Person *) t->val.v;
		free_dllist(x->Children);
		free(x->Name);
		free(x);
	}
	jrb_free_tree(people);

	return 0;
}

char *get_name(IS is) {
	char *name;
	int nsize, i;
	nsize = strlen(is->fields[1]);
    for (i = 2; i < is->NF; i++) nsize += (strlen(is->fields[i])+1);
        name = (char *) malloc(sizeof(char)*(nsize+1));
        strcpy(name, is->fields[1]);
        nsize = strlen(is->fields[1]);
    for (i = 2; i < is->NF; i++) {
        name[nsize] = ' ';
        strcpy(name+nsize+1, is->fields[i]);
		nsize += strlen(name+nsize);
    }
	return name;
}

Person *new_person(char *name, JRB people) {
	Person *x;
	x = malloc(sizeof(Person));
	x->Name = strdup(name);
	x->Children = new_dllist();
	x->visited = 0;
	jrb_insert_str(people, name, new_jval_v((void *) x));
	return x;
}

void add_child(Person *p, Dllist children) {
    Dllist sap;
	Person *t;
	int g;

	if (dll_empty(children) == 1) { 
		dll_append(children, new_jval_v(p));
	} else {
		dll_traverse(sap, children) {
			t = (Person *) sap->val.v;
			if (strcmp(t->Name, p->Name) == 0) {
				g = 1;
			}
		}
		if (g == 0) {
			dll_append(children, new_jval_v(p));
		}
	}
}

int is_descendant(Person *p) {
    Dllist t;
	Person *c;
	if (p->visited == 1) { return 0; }
	if (p->visited == 2) { return 1; }

	p->visited = 2;
	dll_traverse(t, p->Children) {
		c = (Person *) t->val.v;
		if (is_descendant(c)) {
			return 1;
		}
	}
	p->visited = 1;
	return 0;
}

void print_order(Person *x) {
	if (x == NULL) return;
	print_order(x->Father);
	print_order(x->Mother);
	if (x->visited == 0)
		print_fam(x);
}

void print_fam(Person *x) {

		char *mom_name;
		char *dad_name;
		Person *mom;
		Person *dad;
 	    Person *child;
	    Dllist children;

        dad = (Person *) x->Father;
        if (dad == NULL) {
            dad_name = "Unknown";
        } else { dad_name = dad->Name; }
        mom = (Person *) x->Mother;
        if (mom == NULL) {
            mom_name = "Unknown";
        } else { mom_name = mom->Name; }

        printf("%s\n", x->Name);
        if (x->Sex != NULL) {
            printf("  Sex: %s\n", x->Sex);
        } else {
            printf("  Sex: Unknown\n");
        }

        printf("  Father: %s\n" ,dad_name);
        printf("  Mother: %s\n" ,mom_name);
        printf("  Children:");
        if (dll_empty(x->Children) != 1) {
            dll_traverse(children, x->Children) {
                child = (Person *) children->val.v;
                printf("\n    %s", child->Name);
            }
            printf("\n\n");
        } else { printf(" None\n\n"); }
		x->visited = 1;
}

