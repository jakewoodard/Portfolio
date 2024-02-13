#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

// class for stats and member variables/functions
class stats {
	public:
		// variables for statistics
		int N;
		int min;
		int max;
		int sum;
		
		// function declarations
		int push(int);
		void print();
};

// function for calculation/processing of new input
int stats::push(int j) {
	N++; // increment counter
	
	// if statement to set max
	if ((j > max) || (N == 1)) {
		max = j;
	}

	// if statement to set min
	if ((j < min) || (N == 1)) {
		min = j;
	}

	// add to sum
	sum = sum + j;

	// return values
	return N;
	return sum;
	return min;
	return max;
}

// function to print out statistics after input is over
void stats::print() {
	cout << "N   = " << N << endl;
	cout << "sum = " << sum << endl;
	cout << "min = " << min << endl;
	cout << "max = " << max << endl;
}

int main () {
	int input; // set variables for cin
	stats k;
	k.N = 0;
	k.sum = 0;
	k.min = 0;
	k.max = 0;

	// while loop to read in integers from user
	while (cin >> input) {
		k.push(input); // call function to calculate stats
	}
	k.print(); // call function to print
	return 0;
	// end program
}
