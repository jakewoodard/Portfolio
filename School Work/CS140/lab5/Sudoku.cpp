#include <sys/time.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

// NOTE: I would like to use a 
// grace day for this lab. Thank You!

// global units[]
const char *units[3] = { "sec", "msec", "usec" };

class omega {
  public:
	omega();
	float get_sec();

  private:
    long Tinit;
    long Tmark;
};

omega::omega() {
  struct timeval T;
  gettimeofday(&T, NULL);

  Tinit = 1000000*T.tv_sec + T.tv_usec;
}

float omega::get_sec() {
  struct timeval T;
  gettimeofday(&T, NULL);

  Tmark = 1000000*T.tv_sec + T.tv_usec;

  return (float)(Tmark-Tinit)/1000000.0;
}

string elapsed(float duration, int i = 0) {
  if (duration < 0.1) { 
    duration *= 1000;
    i += 1;
    return elapsed (duration, i);
  }

  string s;
  ostringstream oss;
  oss << "Time : " << duration << " " << units[i];
  
  s = oss.str();
  return s;
}

class sudoku {
  public:
	sudoku();

	void solve();

    void read(const char *);
    void write(const char *);
    void write(const char *, const char *);

  private:
    bool solve(vector<int>, int);

    // various support functions 
    bool error_check_value(bool);
    bool error_check_uniqueness();
    bool check_row(int, int);
    bool check_column(int, int);
    bool check_square(int, int, int);
    void display();
    vector<int> valid_values(int, int);

    int game[9][9];
};

// functions for checking 
// rows, columns, and sqaures
bool sudoku::check_row(int r, int v) {
  int check = 0;
  for(int x = 0; x < 9; x++) {
    if (game[r][x] == v) check += 1;
  }
  if (check > 1) return false;
  return true;
}

bool sudoku::check_column(int c, int v) {
  int check = 0;
  for(int x = 0; x < 9; x++) {
    if (game[x][c] == v) check += 1;
  }
  if (check > 1) return false;
  return true;
}

bool sudoku::check_square(int i, int j, int v) {
  int check = 0;
  int ei, ej, si, sj;
  // start of square
  i = (i/3)*3; 
  j = (j/3)*3;
  si = i;
  sj = j;
  // end of square
  ei = i + 2; 
  ej = j + 2;
  while (i <= ei) {
    while (j <= ej) {
      if (game[i][j] == v) { check++; }
      j++;
    }
    j = sj;
    i++;
  }
  if (check == 1 || check == 0) return true;
  return false;
}

vector<int> sudoku::valid_values(int i, int j) {
  int t = game[i][j];
  vector<int> p;
  for (int k = 1; k < 10; k++) {
      game[i][j] = k;
      if (check_row(i, k) && check_column(j, k) && check_square(i, j, k))
        p.push_back(k);
  }
  game[i][j] = t;
  return p;
}

sudoku::sudoku() { 
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++)
      game[i][j] = 0;
  }
}

bool sudoku::error_check_value(bool x) {
  bool err = false;

  // for unsolved puzzle
  if (x == true) {
    for (int i=0; i<9; i++) {
      for (int j=0; j<9; j++) {
        if (game[i][j] > 9 || game[i][j] < 0) 
	  err = true;
      }
    }
  }

  // for solved puzzle
  if (x == false) {
    for (int i=0; i<9; i++) {
      for (int j=0; j<9; j++) {
        if (game[i][j] > 9 || game[i][j] < 1)
          err = true;
      }
    }
  }

  return err;
}

bool sudoku::error_check_uniqueness() {
  bool err = false;
  int v; 
  int check = 0;
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      v = game[i][j];
      if (v > 0) {
        if (!check_row(i, v)) err = true;
        if (!check_column(j, v)) err = true;
        if (check_square(i, j, v) == 0) err = true;
        if (err == true) {
          check += 1;
          cerr << "cell " << i << " " << j
               << " nonunique value " << v
               << endl;
        }
        err = false;
      }
    }
  }
  if (check > 0) err = true;
  return err;
}

void sudoku::solve() {
  cout << "SOLVE\n";
  
  vector<int> cells;
  bool err = false;
  int num;

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (game[i][j] == 0) {
        num = i * 9 + j;
        cells.push_back(num);
      }
    }
  }

  // call recursive solve function
  if (solve(cells, 0))
    display();
  
  // error check data values
  if (error_check_value(false)) err = true;
  // error check uniqueness
  if (error_check_uniqueness()) err = true;
  // exit if errors detected
  if (err == true) exit(0);
}

void sudoku::read(const char *fname) {
  bool err = false;
  int line = 1;

  cout << "READ\n";

  ifstream fin(fname);
    
  int i, j, v;

  while (fin >> i >> j >> v) {
    // error check for i and j
    if (i < 0 || i > 8 || j < 0 || j > 8) {
	err = true;
	cerr << "line " << line << ": " << i 
	     << " " << j << " " << v 
	     << " out-of-bounds grid index\n";
    }

    game[i][j] = v;
    line += 1; // increment line
  }

  fin.close();

  // exit if bad grid indices
  if (err == true) exit(0);

  // call function to display
  display();

  // error check data values
  if (error_check_value(true)) err = true;
  // error check uniqueness
  if (error_check_uniqueness()) err = true;

  // exit if errors detected
  if (err == true) exit(0);
}

void sudoku::write(const char *fname) {
  ofstream fout(fname);

  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      if (0 < game[i][j]) {
        fout << i << " " << j << " "
	     << game[i][j] << "\n";
      }
    }
  }

  fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
  int N1 = strlen(fname);
  int N2 = strlen(addon);

  char *n_fname = new char[N1+N2+2];

  // strip .txt suffix, then concatenate _addon.txt
  strncpy(n_fname, fname, N1-4);
  strcpy(n_fname+N1-4, "_");
  strcpy(n_fname+N1-3, addon);
  strcpy(n_fname+N1-3+N2, ".txt");

  write(n_fname);

  delete [] n_fname;
}

void sudoku::display() {
  cout << "| --------------------------- |\n";
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
	  if (j == 0) 
	    cout << "| " << game[i][j];
	  else if (j%3 == 0) 
	    cout << " | " << game[i][j];
	  else
	    cout << "  " << game[i][j];
	}
    cout << " |\n";
	if (i%3 == 2)
      cout << "| --------------------------- |\n";
  }
}

bool sudoku::solve(vector<int> cells, int c) {
  if (c == cells.size())    // if solution found, 
    return true;            // return solution-found 
     
  int lowest_vv = INT_MAX;  // lowest valid value
  int lowest_vv_i = 0;      // index of cell w/ lowest valid value

  for (int i = c; i < cells.size(); i++) {
    int t = valid_values(cells[i] / 9, cells[i] % 9).size();
    if (t < lowest_vv) {
      lowest_vv_i = i;
      lowest_vv = t;
    }
  }
  
  // swap with index of cell with least 
  // possible valid values
  std::swap(cells[c], cells[lowest_vv_i]);

  // set cell index (i,j)
  int i = cells[c] / 9;
  int j = cells[c] % 9;

  // determine valid values
  vector<int> values = valid_values(i, j);

  if (values.size() == 0) // if no valid values left,
    return false;         // return road-to-nowhere

  // iterate thru valid values 
  for (int k = 0; k < values.size(); k++) {
    game[i][j] = values[k];        // game[i][j] = next value
      if (solve(cells, c+1))       // if solve(arguments) == solution-found
        return true;               //     return solution-found
  }

  game[i][j] = 0; // reset upon failure
  return false;   // return road-to-nowhere
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if ((argc != 3) ||
      (strcmp(argv[1], "-s") != 0) ||
	  strstr(argv[argc-1], ".txt") == NULL) {
    cerr << "usage: Sudoku -s game.txt\n";
	exit(0);
  }

  omega timer;
  sudoku sudoku_game;

  float T0 = 0.0;
  float T1 = 0.0;

  if (strcmp(argv[1], "-s") == 0) {
    sudoku_game.read(argv[2]);

	T0 = timer.get_sec();
    sudoku_game.solve();
	T1 = timer.get_sec();

    sudoku_game.write(argv[2], "solved");
  }

  cout << "\n";
  string elapsed_time = elapsed(T1-T0);
  // print string elapsed_time using two decimal places
  // print T1-T0 using six decimal places for comparison
  cout << elapsed_time << " (" << (T1-T0) << " "
       << units[0] << ")\n";
  return 0;
}
