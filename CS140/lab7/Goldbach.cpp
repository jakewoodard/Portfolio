#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <iterator>
using namespace std;

class prime_partition {
  public:
    // constructor
    prime_partition();

    // operator overload
    void operator()(int);

  private:
    void create_pset();
    bool is_prime(int);
    void compute_partitions(vector<int> &, const int &, int = 0);
    void print_partitions();

    // miscl member data
    set<int> pset; // primes 2, 3, .... 1999
    int pset_min;  // first prime in pset
    int pset_max;  // last prime in pset
    int max_terms; // max primes allowed in sum

    vector<vector<int> > partitions;
};

// constructor
prime_partition::prime_partition() {
  create_pset();
  pset_min = *min_element(pset.begin(), pset.end());
  pset_max = *max_element(pset.begin(), pset.end());
}

// operator overload
void prime_partition::operator()(int a) {
  if (a < 2 || a > 1999) return; // goldilocks check
  if (a % 2 == 1) max_terms = 3; // if odd
  if (a % 2 == 0) max_terms = 2; // if even
  
  partitions.clear(); // clear partitions
  vector<int> dummy;  // dummy vector to hold primes
                      // mid-recursion

  // compute and print partitions
  compute_partitions(dummy, a);
  print_partitions();
}

// create pset
void prime_partition::create_pset() {
  pset.clear();
  bool fun;
  for (int a = 2; a < 2000; a++) { 
    fun = is_prime(a);
    if (fun == true) pset.insert(a);
  }
}

// functions that determine whether int is prime
bool prime_partition::is_prime(int a) {
 int r = sqrt(a);
 for (int i = 2; i <= r; i++) {
   if (a % i == 0) {
     return 0;
   }
 }
 return true;
}

void prime_partition::compute_partitions(vector<int> &n, const int &t, int s) {
  set<int>::iterator start = pset.begin();
  set<int>::iterator ii;

  // if sum equals target
  if (t == s) {
    if (n.size() < max_terms) {
      max_terms = n.size();
      partitions.clear();
      partitions.push_back(n);
    } else if (n.size() == max_terms) {
      partitions.push_back(n);
    }
    return;
  }
  
  if (s > t || n.size() > max_terms) return;  

  // set "end" iterator & for loop thru preset
  // call recursion
  if (n.empty()) {
    set<int>::iterator end = pset.upper_bound(t);
    for (ii = start; *ii != *end; ii++) { 
      n.push_back(*ii);
      compute_partitions(n, t, s + *ii);
      n.pop_back();
    }
  } else {
    set<int>::iterator end = pset.upper_bound(n.back());
    for (ii = start; *ii != *end; ii++) {
      n.push_back(*ii);
      compute_partitions(n, t, s + *ii);
      n.pop_back();
    }
  }
  
}

void prime_partition::print_partitions() {
  for (int i = 0; i < partitions.size(); i++) {
    for (int j = 0; j < partitions.at(i).size(); j++) {
      cout << partitions.at(i).at(j) << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[])
{
  prime_partition goldbach;

  int number;
  while (1) {
	cout << "number = ";
    cin >> number;
	if (cin.eof())
	  break;
	
	goldbach(number);
  }

  cout << "\n";
}
