#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iterator>
#include<iomanip>
#include<stdlib.h>
#include<algorithm>
#include<ctime>
using namespace std;

class data {
  public:
    // add operator< using lastname, firstname, phone number
    bool operator<(const data &) const;

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};

bool data::operator<(const data &j) const {
  if (lastname != j.lastname)
    return lastname < j.lastname;
  if (firstname != j.firstname) 
    return firstname < j.firstname;
  // else
    return phonenum < j.phonenum;
}

istream & operator>>(istream &in, data &r) { 
  // read data object
  in >> r.firstname >> r.lastname >> r.phonenum; 
  return in;
}

ostream & operator<<(ostream &out, const data &r) {
  // write data object
  out << setw(24) << left << r.lastname + ", " + r.firstname
      << setfill(' ') << r.phonenum;
  return out;
}

void quicksort(vector<data> &A, int left, int right) { 
  if (left < right) { 
    int i = left;   // variables to increment and
    int j = right;  // decrement for below
    int x = right - left; 

    // find random pivot
    srand(time(NULL));
    int pindex = rand() % x;
    pindex += left;
    data pivot = A[pindex];

    // move pivot to end of vector
    swap(A[pindex], A[right]);

    while (1) { 
      while (A[i] < pivot) { ++i;  }
      while (pivot < A[j]) { --j; }
      if (i >= j) { pindex = j; break; }
      swap(A[i], A[j]);      
    }
    
    // move pivot back
    // pindex = i;
    // swap(A[pindex], A[right]);
    

    quicksort(A, left, pindex - 1);
    quicksort(A, pindex + 1, right);
  }
}

/*
template <typename Tdata>
void quickselect(...) { 
  // write this 
}
*/

void printlist(typename vector<data>::iterator a, typename vector<data>::iterator b) {
  int p = 0;
  while (a != b) { cout << *a << "    " << "\n"; a++; p++; }
}

int main(int argc, char *argv[]) {
  // perform command-line error checking
  // usage: Qsort -stl | -rpivot [k0 k1] file.txt
  if (argc != 3 && argc != 5) {
    cerr << "usage: Qsort -stl | -rpivot [k0 k1] file.txt\n";
    return -1;
  }
  
  string filename;
  filename = argv[argc-1];
  string mode;
  mode = argv[1];
  
  if (mode != "-stl" && mode != "-rpivot") {
    cerr << "usage: Qsort -stl | -rpivot [k0 k1] file.txt\n";
    return -1;
  }

  // open file.txt 
  ifstream fin(filename);

  vector<data> A;

  data din;
  while (fin >> din)
    A.push_back(din);

  // close file.txt
  fin.close();
  
  if (mode == "-stl") {
    std::sort(A.begin(), A.end());
  }
  
  if (mode ==  "-rpivot") {
    int N = (int)A.size();
    int k0, k1;

    if (argc == 3) { 
      k0 = 0;
      k1 = N - 1;
    }

    // if specified, update k0, k1 and apply quickselect 
    if (argc == 5) {
      k0 = atoi(argv[2]);
      k1 = atoi(argv[3]);
    }

    // and ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]
    if (k0 <= 0) k0 = 0;
    if (k1 >= N) k1 = N-1;
    
    quicksort(A, k0, k1);
  } 
  
  printlist(A.begin(), A.end());
  
  return 0;
}
