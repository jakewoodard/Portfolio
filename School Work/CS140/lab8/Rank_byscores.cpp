#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <numeric>
#include <utility>
#include <algorithm>
using namespace std;

class name_t {
  public:
    name_t();
    name_t(const string &, const string &);

    bool operator<(const name_t &) const;
    void print(int = 0) const;

  private:
    string name;
};

class scores_t {
  public:
    scores_t();

    void insert(int);
    void insert_done();
    void print();
    float return_mean() const;

  private:
    vector<int> scores;
    float mean;
};

class namescores_t {
  public:
    namescores_t();
    namescores_t(const name_t &, const scores_t &);

    bool operator<(const namescores_t &) const;
    
    void print_name(int);
    void print_scores();
  
  private:
    name_t n;
    scores_t s;
};


name_t::name_t() {
  name = "";
}

name_t::name_t(const string &a, const string &b) {
  name = b;
  name += ", ";
  name += a;
}

bool name_t::operator<(const name_t &a) const {
  return name < a.name;
}

void name_t::print(int w) const {
  cout << setw(w + 3) << setfill('.') << left << name + ' ';
}

scores_t::scores_t() {
  mean = 0.0;
}

void scores_t::insert(int a) {
  scores.push_back(a);
}

void scores_t::insert_done() {
  int init = 0;
  int size = scores.size();
  mean = accumulate(scores.begin(), scores.end(), init);
  if (size > 0)
    mean /= size;
}

void scores_t::print() {
  cout << setfill(' ') << right;
  int size = scores.size();
  for (int i = 0; i < size; i++) {
    cout << " " << setw(2) << scores[i];
  }
  cout << " : " << setprecision(1) << fixed << mean << endl;
}

float scores_t::return_mean() const {
  return mean;
}

namescores_t::namescores_t(const name_t &a, const scores_t &b) {
  n = a;
  s = b;
}

bool namescores_t::operator<(const namescores_t &t) const {
  float a = s.return_mean();
  float b = t.s.return_mean();
  
  
  bool c = t.n < n;

  if (a != b) {
    return a < b;
  } else {
    return c;
  }
}

void namescores_t::print_name(int a) {
  n.print(a);
}

void namescores_t::print_scores() {
  s.print();
}

int main(int argc, char *argv[]) {
  
  // commandline parsing
  int w, k;
  string filename;
    
  // argv[1]: w (width of name field)
  w = atoi(argv[1]);
  // argv[2]: k (number of elements to print)
  k = atoi(argv[2]);
  // argv[3]: filename.txt
  filename = argv[3];

  // open filename.txt
  ifstream fin;
  fin.open(filename);
  
  // vector of namescores (new class that stores a 
  // name_t/scores_t pair) NO MORE MAP (Balanced BST)
  vector<namescores_t> NS;

  // variables for file reading
  string first, last, line;
  int score;

  while (getline(fin, line)) {
    
    istringstream iss(line);
    iss >> first >> last;

    // add first and lastname to name_t object
    name_t n(first, last);
    scores_t s;
    
    // while more to read...
    while (!(iss.eof())) {
      // insert score into scores_t object
      iss >> score;
      s.insert (score);
    }
    
    // calculate mean score
    s.insert_done();

    // clear stringstream
    iss.str(string());
   
    // create and insert namescores_t object into NS list
    namescores_t t(n, s);
    NS.push_back(t);
  }

  // close filename.txt
  fin.close(); 

  // turn NS list into binary max-heap
  make_heap(NS.begin(), NS.end());

  // loop through and print elements in order of score
  for (int i = 0; i < k && !(NS.empty()); i++) {
    NS[0].print_name(w);
    NS[0].print_scores();
    pop_heap(NS.begin(), NS.end());
    NS.pop_back();
  }
}
