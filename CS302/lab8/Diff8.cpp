#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <cstring>
using namespace std;

// global ints
const int VERT = 1;
const int HORZ = 2;
const int DIAG = 4;

template <typename T>
class matrix {
  // class for storing 2D array
  public: 
    matrix() { data = NULL; }

    ~matrix() {
      if (data) {
        if (data[0])
          delete [] data[0];
        delete [] data;

        data = NULL;
      }
    }
  
    void assign(int _Nrows, int _Ncols, const T &value=T()) {
      Nrows = _Nrows;
      Ncols = _Ncols;

      data = new T *[Nrows];
      data[0] = new T [Nrows*Ncols];
      for (int i=1; i<Nrows; i++)
        data[i] = data[i-1] + Ncols;

      for (int i=0; i<Nrows; i++) {
        for (int j=0; j<Ncols; j++)
          data[i][j] = value;
      }
    }

    int get_Nrows() const { return Nrows; }
    int get_Ncols() const { return Ncols; }

    T * operator[] (int i) { return data[i]; }

  private:
    int Nrows, Ncols;
    T **data;
};

class LCS {
  public:
    void text1_push_back(string);
    void text2_push_back(string);
    void compute_alignment();
    void report_difference() { report_difference(m,n); }

  private:
    // support functions
    void report_difference(int &, int &);
    int DELcost(string s) { return 1; }
    int INScost(string s) { return 1; } 
    int SUBcost(string s1, string s2) { return s1 == s2 ? 0 : m+n; }

    int m, n; 
    matrix<int> cost; 
    matrix<int> link;
    stack<string> alignment_1;
    stack<string> alignment_2;

    // buffers
    vector<string> v1;
    vector<string> v2;
};

void LCS::text1_push_back(string s1) {
  v1.push_back(s1);
  return;
}

void LCS::text2_push_back(string s2) {
  v2.push_back(s2);
  return;
}

void LCS::compute_alignment() {
  m = v1.size();
  n = v2.size(); 

  cost.assign(m+1, n+1, 0);
  link.assign(m+1, n+1, 0);

  cost[0][0] = 0;
  link[0][0] = 0;
  
  for (int i=1; i<m; i++) {
    cost[i][0] = cost[i-1][0] + DELcost(v1[i]);
    link[i][0] = VERT;
  }

  for (int j=1; j<n; j++) {
    cost[0][j] = cost[0][j-1] + INScost(v2[j]);
    link[0][j] = HORZ;
  }
  
  for (int i=1; i<m; i++) {
    for (int j=1; j<n; j++) {
      cost[i][j] = cost[i-1][j-1] + SUBcost(v1[i],v2[j]);
      link[i][j] = DIAG;
     
      int delcost = cost[i-1][j] + DELcost(v1[i]);
      if (delcost < cost[i][j]) {
        cost[i][j] = delcost;
        link[i][j] = VERT;
      }

      int inscost = cost[i][j-1] + INScost(v2[j]);
      if (inscost < cost[i][j]) {
        cost[i][j] = inscost;
        link[i][j] = HORZ;
      }
    }
  }
  return;
} 

void LCS::report_difference(int &a, int &b) {
  int i=a, j=b, link_ij=link[i][j];

  if (link_ij == DIAG) {
    alignment_1.push(v1[i]);
    alignment_2.push(v2[j]);
    i = i-1, j = j-1;
    report_difference(i, j);
  } else if (link_ij == VERT) {
    alignment_1.push(v1[i]);
    alignment_2.push(v2[0]);
    i = i-1;
    report_difference(i, j);
  } else if (link_ij == HORZ) {
    alignment_1.push(v1[0]);
    alignment_2.push(v2[j]);
    j = j-1;
    report_difference(i, j);
  } else { 
/*
	i = 0; j = 0; string c, d;
    while (!alignment_1.empty() && !alignment_2.empty()) {
      c = alignment_1.top(); d = alignment_2.top();
      if (c != d) {
	if (d.length() > 1) cout << "> " << d << endl; 
        if (c.length() > 1) cout << "< " << c << endl;
      }
      alignment_1.pop(); alignment_2.pop();
      c = ""; d = "";
    }
*/

  }
  return;
}

int main(int argc, char *argv[]) {

  // check two input files are specified on command line
  if (argc != 3) {
    cerr << "usage: ./Diff8 file1.txt file2.txt\n";
    return -1;
  }
  string f1 = argv[1];
  string f2 = argv[2];

  ifstream fin1(f1.c_str());
  ifstream fin2(f2.c_str());
  if (fin1.is_open() == 0) {
    cerr << "error opening file1.txt\n";
    return -1;
  }
  if (fin2.is_open() == 0) {
    cerr << "error opening file2.txt\n";
    return -1;
  }

  LCS lcs;  // instantiate your "LCS based diff" object
  string a = "";  string b = "";
  lcs.text1_push_back(a);
  lcs.text2_push_back(b);

  // read the text from file1 into the lcs.text1 buffer
  while(getline(fin1, a)) {
    lcs.text1_push_back(a);
    a = "";
  }

  // read the text from file2 into the lcs.text2 buffer
  while(getline(fin2, b)) { 
    lcs.text2_push_back(b);
    b = "";
  }
  
  lcs.compute_alignment();
  lcs.report_difference();

  cout << "Sorry, was never able to get it printing correctly...\n";
  return 0;
}
