// include header file(s) needed
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<stdlib.h>
using namespace std;

#include "Slist.h"

// modify the data class and the associated input 
// and output operators to store, set and print a 
// line number identifier called ID

// global integer for doing so ^^^
int c = 0;

class data { 
  // copy from QsortA.cpp 
  public:
    // compare lastname then firstname then phone #
    bool operator<(const data &) const;

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);
  
  private: 
    string firstname;
    string lastname;
    string phonenum;

    // unique ID (line number) 
    int ID;
};

// overloaded comparison operator
bool data::operator<(const data &j) const {
  if (lastname != j.lastname)
    return lastname < j.lastname;
  if (firstname != j.firstname)
    return firstname < j.firstname;
  // else
    return phonenum < j.phonenum;
}

// istream & operator>> (...)
istream & operator>>(istream &in, data &r) {
  // read data object
  r.ID=++c;
  in >> r.firstname >> r.lastname >> r.phonenum;
  return in;
}

// ostream & operator<< (...) 
ostream & operator<<(ostream &out, const data &r) {
  // write data object
  out << setw(24) << left << r.lastname + ", " + r.firstname
      << setfill(' ') << r.phonenum << setw(9) << right << r.ID;
  return out;
}

// void printlist(...) { copy from QsortA.cpp }
void printlist(typename slist<data>::iterator a, typename slist<data>::iterator b) {
  while (a != b) { cout << *a << "    " << "\n"; ++a; }
}

int main(int argc, char *argv[]) {
  // copy command-line check from Qsort.cpp,
  // then modify to apply to this assignment
  
  if (argc != 3) {
    cerr << "usage: Slist -mergesort | -quicksort | -qsort_r3 file.txt\n";
    return -1;
  }

  string filename;
  filename = argv[argc-1];
  string mode;
  mode = argv[1];
  
  if (mode != "-mergesort" && mode != "-quicksort" && mode != "-qsort_r3") {
    cerr << "usage: Slist -mergesort | -quicksort | -qsort_r3 file.txt\n";
    return -1;
  }

  // open input file
  ifstream fin(filename);
  
  // list and data object for read-in
  slist<data> A;
  data din;

  while (fin >> din) {
    A.push_back(din); 
  }
  // close input file
  fin.close();

  A.sort(mode);

  printlist(A.begin(), A.end());
  return 0;
}
