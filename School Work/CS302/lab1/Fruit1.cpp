#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
using namespace std;

struct fruit { 
  string name;
  float unit_cost;
  float quantity;
  // overload operator <
  bool operator<(const fruit &) const;
};

bool fruit::operator<(const fruit &a) const 
{ return name < a.name; }

int main(int argc, char *argv[])
{ 

  // variable for total grocery list cost
  float grocery_cost = 0;

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

  // declare array_list<fruit>
  vector<fruit> fruit_data; 

  // open file
  ifstream fin(filename);

  // variables for file reading
  string fruit_name, line;
  float q, c;
  fruit f;

  while (getline(fin, line)) {
    // store data INORDER
    istringstream iss(line);
    iss >> fruit_name >> q >> c;
    
    // set up fruit object
    f.name = fruit_name;
    f.unit_cost = c;
    f.quantity = q;
    
    // insert
    fruit_data.push_back(f);

    // clear stringstream
    iss.str(string());
  }

  if (mode == "-sortall") { 
    // apply std::stable_sort
    stable_sort(fruit_data.begin(), fruit_data.end());
  }

  if (mode == "-sortone") {
    // apply std:: stable_sort
    stable_sort(fruit_data.begin(), fruit_data.end());

    // combine fruits with same name
    int i = 0;
    vector<fruit> new_fruit_data;
    vector<fruit>::iterator it;
    it = fruit_data.begin();
    it++;

    while (1) {  
      fruit f1 = fruit_data[i];
      
      while (f1.name == fruit_data[i+1].name) {
        f1.quantity += fruit_data[i+1].quantity;
        i++;
        // fruit_data[i].quantity += fruit_data[i+1].quantity;
        // fruit_data.erase(it);
        // fruit_data.resize(fruit_data.size());
      }
      new_fruit_data.push_back(f1);
      i++;
      // it++;
      if (i == fruit_data.size()) break;
    } 
    fruit_data.clear(); 
    fruit_data = new_fruit_data;
  }
   
  // print vector contents
  while (fruit_data.size() != 0) {
    float x; 
    x = fruit_data[0].quantity * fruit_data[0].unit_cost;
    grocery_cost += x; 

    // pretty-print array list content to stdout
    cout << setw(20) << setfill('.') << left << fruit_data[0].name << "  "
         << setw(5) << setfill(' ') << right << setprecision(2) << fixed 
         << fruit_data[0].quantity << " x " << setw(4) << right 
         << setprecision(2) << fixed << fruit_data[0].unit_cost << " = "
         << setw(7) << right << setprecision(2) << fixed 
         << fruit_data[0].quantity * fruit_data[0].unit_cost
         << " :" << setw(8) << right << setprecision(2) 
         << fixed << grocery_cost << "\n";

    // free array list memory as necessary
    fruit_data.erase(fruit_data.begin());
    fruit_data.resize(fruit_data.size());
  }
  fin.close();
  return 0;
}
