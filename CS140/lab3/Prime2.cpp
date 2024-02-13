#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

class isprime {
	public:
    	isprime() {  }
	bool magic(int);
    	bool operator()(int);

  	private:
	int max = 2;
	vector<int> primelist {2};
	vector<int>::iterator it;
};

bool isprime::magic(int number)
{
	int flag = 0;
	int r = sqrt(number);
        
	for (int i = 2; i <= r; i++) {
                if ((number % i) == 0) {
			flag = 1;
			break;
		}
	}

	for (int i = max; i <= number; i++) {
		int keep = 0;
		for (int j = 2; j < max; j++) {
			if (i % j == 0) keep++;
		}
		it = find(primelist.begin(), primelist.end(), i);
                if (it == primelist.end() && keep == 0 && i % 2 != 0)
                        primelist.push_back(i);
	}
	
	if (flag == 0) { return true; }
	return false;
}

bool isprime::operator()(int number)
{
	bool fun = magic(number);
	if (max < number) {
		max = number;
	}

	if (fun == 0) return false;
	if (fun == 1) return true; 
	return 0;
}

int main() {
	isprime pcheck;
	int number;

	while (cin >> number) {
	  if (pcheck(number)) cout << number << " is a prime number\n";
	}
return 0;
}
