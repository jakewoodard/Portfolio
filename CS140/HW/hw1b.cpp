I#include <iostream>
#include <cstdlib>
using namespace std;

int strlength(const char *c) {
        int a = 0;
        for (const char *i = c;; i++) {
                if (*i == '\0') {
                        break;
                }
                a++;
        }
        return a;
};

int main(int argc, char *argv[]) {
	
	cout << "Num args = " << argc << endl;
	
	int j;
	j = argc - 1;

	int i;
	i = 0;

	while (j >= 0) {
		
		cout << "argv[" << i <<  "] = " << &argv[i] 
		<< " " << argv[i] << " (strlen=" << strlength(argv[i])  
		<< ")" << endl;
		
		i++;
		j--;
	}
}
