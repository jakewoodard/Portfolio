#include <iostream>
#include <ctype.h>
#include <string>
using namespace std;

// encode only lower and upper case letters by j
void encode(string &s, int j) { 
	for (int i = 0; i < s.size(); i++) {
		
		// iterate when upon a non alphabet ascii value
		if (isalpha(s[i]) == 0)
			continue;

		// encode lower case AND
		// fix unicode issue w ascii value check for wxyz 
		if (islower(s[i]) != 0) {
			if ((s[i] > 118) && (j >= 6)) {
				s[i] = s[i] - 26;
				s[i] = s[i] + j;
			}
			else {
				s[i] = s[i] + j;
				if (s[i] > 122) {
					s[i] = s[i] - 26;
				}
			}
		}

		// encode upper case
		if (isupper(s[i]) != 0) {
			s[i] = s[i] + j;
			if (s[i] >= 91) {
                                s[i] = s[i] - 26;
                        }
		}
	}
return;
};

// decode alphabeticals by j 
void decode(string &s, int j) {        
	for (int i = 0; i < s.size(); i++) {

                // iterate when upon a non alphabet ascii value
                if (isprint(s[i]) == 0)
                        continue;
                if (isalpha(s[i]) == 0)
                        continue;

                // decode lower case
                if (islower(s[i]) != 0) {
                        s[i] = s[i] - j;
                        if (s[i] <= 96) {
                                s[i] = s[i] + 26;
                        }
                }

                // encode upper case
                if (isupper(s[i]) != 0) {
                        s[i] = s[i] - j;
                        if (s[i] <= 64) {
                                s[i] = s[i] + 26;
                        }
                }
        }
return; 
};

int main (int argc, char *argv[]) {

	// end program if argc does not equal 3 
	if (argc != 3) {
		cerr << "Error: argc MUST equal 3" << endl;
		return 0;
	}
	
	// end program if argv[1] does not equal "-decode" or "-encode"
	string x ("-encode");
	string y ("-decode");
	if ((x.compare(argv[1]) != 0) && (y.compare(argv[1]) != 0)) {
		cerr << "Error: must input -encode or -decode for argv[1]" 
		     << endl;
		return 0; 
	}
	
	
	// end program if argv[2] is not an integer
	int z = *argv[2];
	if ((z <= 47) || (z >= 58)) {
		cerr << "Error: argv[2] must be a single digit integer" 
		     << endl;
		return 0;
	}
	
	// read text into char array
	string text = "";
	char input[2048];
	z = z - 48;
	cin.getline(input, 2048, '\0');

	// convert char array to string for encoding/decoding
	for (int k = 0;; k++) {
		if (input[k] == '\0') 
			break;
		text = text + input[k];
	}
		
		// encode
		if (x.compare(argv[1]) == 0) {
			encode(text, z);
		}

		// decode
		if (y.compare(argv[1]) == 0) {
			decode(text, z);
                }
	
	// output final string
	cout << text << endl;

return 0;
}
