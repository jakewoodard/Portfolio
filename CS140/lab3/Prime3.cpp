#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

class isprime {
	public:
		isprime() { }
		bool magic(int);
		bool operator()(int);

  	private:
    		int max = 2;
		vector<int> v2 {2};
};

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

bool isprime::magic(int number)
{
        int flag = 0;
        int r = sqrt(number);
	
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
                found = binary_search(v2.begin(), v2.end(), i);
                if (found == false && keep == 0 && i % 2 != 0)
                	v2.push_back(i);
        }

        if (flag == 0) { return true; }
        return false;
}

int random_number() { return rand() % 140 + 1; }

int main(int argc, char *argv[])
{
	isprime pcheck;
	
	int N;
	if (argv[1])  { N = atoi(argv[1]); }
	else { N = 140; }

	srand(N);
	
	vector<int> v1 (N);
	vector<bool> v3 (N);
	
	generate(v1.begin(), v1.end(), random_number);
	transform(v1.begin(), v1.end(), v3.begin(), pcheck);

	int c = count(v3.begin(), v3.end(), true);
	
	cout << "Sequence contains " << c << " prime numbers.\n";
		
	return 0;
}
