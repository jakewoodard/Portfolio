#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;

// Note: I folded my extract_prime function into my print
//       function, my apologies... However, I still implemented
//       what I needed to.

class isprime {
	public:
		isprime() { }
		bool magic(int);
		bool operator()(int);

  	private:
    		int max = 2;        // max number in sequence
		vector<int> v2 {2}; // vector of previously calculated prime numbers
				    // initialized to two 
};

// overloaded operator that calls magic
// to check if int is prime
bool isprime::operator()(int number)
{
	bool fun = magic(number); // call magic
        if (max < number) {
                max = number; // set new max
        }

        if (fun == 0) return false;
        if (fun == 1) return true;
        return 0;
}

// function that determines whether a number is prime or not
bool isprime::magic(int number)
{
        int flag = 0;         // flag for whether a number is prime
        int r = sqrt(number); // squareroot to reduce calc time
	
	if (number == 1) { return false; }

        for (int i = 2; i <= r; i++) {
                if ((number % i) == 0) {
                        flag = 1;
                        break;
                }
        }

        for (int i = max; i <= number; i++) {
                int keep = 0;
		bool found;
		for (int j = 2; j < max; j++) {
                        if (i % j == 0) keep++;
                }
		// call binary search and set = to new bool "found" 
		// to determine whether or not to add to vector v2
                found = binary_search(v2.begin(), v2.end(), i);
                if (found == false && keep == 0 && i % 2 != 0)
                	v2.push_back(i); // add to vector of previously calculated
	}			         // prime numbers

        if (flag == 0) { return true; }
        return false;
}

int random_number() { return rand() % 140 + 1; }

// function to print formatted output
// as well as what prime_extract() was intended
// to do: sort(), unique(), erase(), etc.
void print(vector<int> a, vector<bool> b, int N)
{
	isprime pcheck;
	vector<bool> c (N); // new bool vector for sorted vector of values later	
	int j = 0; // variable to keep track of when to endline
	
	// print out original vector (only prime numbers)
	cout << "All numbers in order appearance:\n";
	for (int i = 0; i < N; i++) {
		if (b.at(i) == true) { 
			cout << setfill(' ') << setw(4) << a.at(i);
			j++;
		}
		if (j == 20) { cout << endl; j = 0; }
	}
	cout << endl;
	
	j = 0; // reset variable

	// sort vector and filter out duplicates
	sort(a.begin(), a.end());
	unique(a.begin(), a.end());
	
	// call transform again to find which values are
	// prime in new vector
	transform(a.begin(), a.end(), c.begin(), pcheck);
	
	// print out sorted and unique prime values in vector
	int d, e;
	cout << "Unique values in numerical order:\n";
	for (int k = 0; k < N; k++) {
		d = a.at(k); e = a.at(k+1);
		if (c.at(k) == true) {
			cout << setfill(' ') << setw(4) << a.at(k);
			j++;
		}
		if (j == 20) { cout << endl; j = 0; }
		
		if (e < d) {
			a.erase(a.begin()+k, a.end());
			b.erase(b.begin()+k, b.end());
			c.erase(c.begin()+k, c.end());
		}
		if (e < d) break;
	}	
	cout << endl;

	// erase all vectors
	c.erase(c.begin(), c.end());
	a.erase(a.begin(), a.end());
	b.erase(b.begin(), b.end());
}
int main(int argc, char *argv[])
{
	isprime pcheck;
	
	int N;
	if (argv[1])  { N = atoi(argv[1]); } // convert char array to int
	else { N = 140; }                    // else set value of N to 140

	srand(N);
	
	vector<int> v1 (N);  // int vector for sequence
	vector<bool> v3 (N); // bool vector correlating to each value 
			     // in sequence
	
	// call generate to determine new random vector of ints
	generate(v1.begin(), v1.end(), random_number);

	// call transform to determine which values in vector v1 are prime
	transform(v1.begin(), v1.end(), v3.begin(), pcheck);

	// count total prime numbers in random sequence
	int c = count(v3.begin(), v3.end(), true);
	
	// output # of primes in sequence
	cout << "Sequence contains " << c << " prime numbers.\n";
	
	// call print
	print(v1, v3, N);	
	return 0;
}
