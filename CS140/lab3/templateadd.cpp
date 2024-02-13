#include <iostream> 
#include <string>
using namespace std;

template <typename T>
T add (const T &v1, const T &v2) { return v1 + v2; }

int main () {
	int i = 14;
	int j = 3;
	float a = 1.25;
	float b = 3.5;
	string c = "knox";
	string d = "ville";

	cout << i << " + " << j << " = " << add(i, j) << "\n";
	cout << a << " + " << b << " = " << add(a, b) << "\n";
	cout << c << " + " << d << " = " << add(c, d) << "\n";

	return 0;
}
