#include <iostream>
#include <cmath>
using namespace std;

bool isprime(int number) {
	int r = sqrt(number);
	for (int i = 2; i <= r; i++) {
		if ((number % i) == 0) { 
			return 0; 
		}
	}
	return true;
};

int main()
{
	int number;
	while (cin >> number) {
	  if (isprime(number))
	    cout << number << " is a prime number\n";
	}
	return 0;
}
