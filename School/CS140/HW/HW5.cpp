#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

// templated class for stats 
template <typename T> 
class stats {
	private: 
		// variables
		int N;
		T sum;
	public:
		// functions 
		stats();
		void push(T &);
		void print();
};

// function to initialize values for sum and N
template <typename T> 
stats<T>::stats() {
	N = 0;
	sum = T();
}

// function to calculate next input
template <typename T> 
void stats<T>::push(T &x) {
        N++; // increment counter

        // add to sum
        sum += x;
}

// function to print out statistics after input is over
template <typename T>
void stats<T>::print() {
	cout << "N   = " << N << endl;
	cout << "sum = " << sum << endl;
}

int main () {
	int input; // set variables for cin
	stats<int> k;

	// while loop to read in integers from user
	while (cin >> input) {
		k.push(input); // call function to calculate stats
	}
	k.print(); // call function to print
	return 0;
	// end program
}
