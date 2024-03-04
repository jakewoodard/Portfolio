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

  private:
    vector<int> scores;
    float mean;
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

int main(int argc, char *argv[]) {

  // commandline parsing
    int w; 
    w = atoi(argv[1]);        //  W (width of name field)
    string filename; 
    filename = argv[2]; //  filename

  // file stream and open
    ifstream fin;
    fin.open(filename);

  // balanced BST
  //   <  key ,  value  >
    map<name_t, scores_t> NS;

  // variables for file reading
  string first, last, line;
  int score;
  
  while (getline(fin, line)) {
    
    istringstream iss(line);
    iss >> first >> last;

    // add first and last name 
    // to name_t object
    name_t n(first, last);
    scores_t s;
    // while more to read...
    while (!(iss.eof())) {
      iss >> score;
      s.insert(score);
    }
    
    // calculate mean score
    s.insert_done();

    // clear string stream
    iss.str(string());
    
    // create and insert (name_t,scores_t) pair
    // into NS map if unique
    if (NS.find(n) == NS.end())
      NS.insert(make_pair(n, s));
  }

  fin.close();

  // iterator, temporary variables, and while 
  // loop for printing map by name alphabetically
  map<name_t, scores_t>::iterator it;
  name_t TEMP;
  scores_t TEMP2;
  it = NS.begin();
  while (it != NS.end()) {
    // print name
    TEMP = it->first;
    TEMP.print(w);
    // print scores
    TEMP2 = it->second;
    TEMP2.print();
    it++;
  }
}

