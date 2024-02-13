#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

struct fruit {
  // overload operators <, <=, ==, and +=
  bool operator<(const fruit &) const;
  bool operator<=(const fruit &) const;
  bool operator==(const fruit &) const;
  void operator+=(const fruit &);
  // data declarations
  string name;
  float unit_cost;
  float quantity;
};

struct node {
  // data declarations
  fruit data;
  node *next;
};

bool fruit::operator<(const fruit &a) const {
  return name < a.name;
}

bool fruit::operator<=(const fruit &a) const {
  return name <= a.name;
}

bool fruit::operator==(const fruit &j) const {
  return this->name == j.name;
}

void fruit::operator+=(const fruit &j) { 
  this->quantity += j.quantity;
} 

int main(int argc, char *argv[])
{
  // prog -inorder|sortall|sortone file.txt
  if (argc != 3) {
    cerr << "usage: ./prog -inorder|sortall|sortone file.txt\n";
    return -1;
  }

  // set program mode from command line arg
  string filename;
  filename = argv[2];
  string mode;
  mode = argv[1];

  // more error checking
  if (mode != "-inorder" && mode != "-sortall" && mode != "-sortone") {
    cerr << "usage: ./prog -inorder|sortall|sortone file.txt\n";
    return -1;
  }
  
  // declare linked_list<fruit>
  node *head = new node();
  node *s = new node();
  head->next = NULL;

  // open file
  ifstream fin(filename);

  // variables for file reading
  string fruit_name, line;
  float q, c;
  fruit f;
  float grocery_cost = 0.0;

  while (getline(fin, line)) { 
    istringstream iss(line);
    iss >> fruit_name >> q >> c;
    
    // set up fruit object
    f.name = fruit_name;
    f.unit_cost = c;
    f.quantity = q;

    // clear stringstream
    iss.str(string());
    
    s = head;
    node *fun = new node();
    fun->data = f;

    // store data INORDER | SORTALL | SORTONE
    if (mode == "-inorder") { 
      while (s->next != NULL) {
        s = s->next;
      }
      s->next = fun;
    } 
    
    if (mode == "-sortall") {
        while (s->next != NULL && s->next->data <= fun->data) {
          s = s->next;
        }
        fun->next = s->next;
        s->next = fun;
    }

    if (mode == "-sortone") {
      if (s->data.name == fun->data.name) {
        s->next->data.quantity += fun->data.quantity;
        break;
      }
      while (s->next != NULL && s->next->data < fun->data) {
        s = s->next;
      }       
      s->next = fun;

    }

  }

  // pretty-print linked list content to stdout
  node *t = head;
  t = t->next;
  while (t != NULL) {
   float x;
   x = t->data.quantity * t->data.unit_cost;
   grocery_cost += x;
   cout << setw(20) << setfill('.') << left << t->data.name
        << " " << setw(5) << setfill(' ') << right 
        << setprecision(2) << fixed << t->data.quantity << " x "
        << setw(4) << right << setprecision(2) << fixed
        << t->data.quantity << " = " << setw(7) << right 
        << setprecision(2) << fixed << t->data.quantity * t->data.unit_cost
        << " :" << setw(8) << right << setprecision(2)
        << fixed << grocery_cost << "\n";
   
   t = t->next; // free linked list memory
  }

  fin.close();
  return 0;
}
