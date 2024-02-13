#include <iostream>
#include <cstdlib>
using namespace std; 

int main() {

	// declare variables for stats
	int num;
	int min = 0;
	int max = 0;
	int sum = 0;
	int N = 0;
	
	// while loop to read in integer
	while (cin >> num) {
		
		N++; // increment counter

		// set max value with if statement
		if ((num > max) || (N == 1)) {
			max = num;
		}
		// set min value with if statement
		if ((num < min) || (N == 1)) {
			min = num;
		}

		// add to sum
		sum = sum + num;
	}
	
	// output stats to user
	cout << "N   = " << N << "\n";
	cout << "sum = " << sum << "\n";
	cout << "min = " << min << "\n";
	cout << "max = " << max << "\n";
}
