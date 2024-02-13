#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]) {
	
	cout << "Num args = " << argc << endl;
	
	int j;
	j = argc - 1;
	
	int i;
	i = 0;

	while (j >= 0) {
		cout << "argv[" << i <<  "] = " << &argv[i] <<
		" " << argv[i] << endl;
		i++;
		j--;
	}

	return 0; 
}
