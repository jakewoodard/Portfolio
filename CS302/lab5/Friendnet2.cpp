#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <set>
#include <algorithm>
#include <iomanip>
#include <fstream>
using namespace std;

void set_oldfriends(vector <string> &name, vector<vector<int> > &friends, int M0, int M1) {
  int M, j;
  int N = (int)name.size();

  // initialize 2D-vector-array friends (hint: use vector::assign())
  friends.assign(N, vector<int> (N,0));

  for (int i=0; i<N; i++) {
    // declare std::set called doknow
    set<int> doknow;
    // initialize M (random number of friends: function of M0, M1)
    M = M0 + rand() % M1;

    while ((int)doknow.size() < M) {
      while (1) {
        // compute j (random friend index: hint j != i)
        j = rand() % N;
        if (j != i) break;
      }
      doknow.insert(j);
    }

    // use iterator to sweep thru doknow set
    set<int>::iterator it;
    // update the corresponding pairs of friends entries
    for (it = doknow.begin(); it != doknow.end(); it++) {
      friends[i][*it] = 1; friends[*it][i] = 1;
    }
  }
  return;
}


void set_newfriends(vector<vector<int> > &friends, vector<vector<int> > &new_friends) {
  int N = (int)friends.size();
  int A, B = 0;
  // initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.assign(N, vector<int> (N,0));
 
  for (int i=0; i<N; i++) {
    // for (each adjacent friend: friends[i][j] == 1) 
    A=0;  B=0;
    for (int j=0; j < N; j++) {
      //  for (each adjacent friend-of-friend: friends[j][k] == 1)
      A = friends[i][j];
      if (A == 1) {
        for (int k=0; k < N; k++) {
	  // if (k-is-not-i && k-is-not-friend-of-i)
          B = friends[j][k];
          if (B == 0) continue;
          if (k != i && friends[i][k] == 0) {
          
	    // update the corresponding pairs of new_friends entries
            new_friends[i][k] = 1;
            new_friends[k][i] = 1;
          }
        }
      }
    }
  }
  return;
}

void writetofile(const char *fname, vector<string> &name, vector<vector<int> > &friends) {
  // open file
  ofstream fout(fname);

  int p = 0; // print counter
  int N = (int)name.size();
  // determine max name length

  for (int i=0; i<N; i++) {
    p = 0;
    fout << setw(11) << left << name[i] << ": ";
    for (int j=0; j < N; j++) {
      if (friends[i][j] == 1) { p++; fout << setw(11) << left << name[j]; }
      if (p % 8 == 0 && p != 0 && friends[i][j] == 1) {
        fout << "\n" << setw(11) << left << name[i] << ": ";
      }
    }
    fout << "\n";
  }

  // close file
  fout.close();
  return;
}

int main(int argc, char *argv[]) {
  // parse argc, argv arguments
  // print usage message and exit if invalid
  // seed random number generator
  int seed;
  if (argc > 2) {
    cerr << "cat datafile.txt | Friendnet1 [-seed N]\n";
    return -1;
  } else if (argc == 2) {
    seed = atoi(argv[1]);
    srand(seed);
  } else { 
    srand(time(NULL));
  }

  // read strings from stdin into name vector
  vector<string> name;
  string din;
  while (cin >> din) {
    name.push_back(din);
  }

  int M0 = 1; // min number of friends
  int M1 = 3; // max number of friends

  // declare 2D-vector-array called friends 
  vector<vector<int> > friends;
  // declare 2D-vector-array called new_friends
  vector<vector<int> > new_friends; // aka friends of friends

  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow1.txt", name, friends);

  set_newfriends(friends, new_friends);
  writetofile("mightknow1.txt", name, new_friends);
  return 0;
}
