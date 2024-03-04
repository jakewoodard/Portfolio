#include <fstream>
#include <iostream>
using namespace std;

int strcompare(const char *s1, const char *s2) {
	const char *S1 = s1;
	const char *S2 = s2;
	for (int i = 0; *S1 != '\0'; i++) {
		if ((*(unsigned char *)S1 != *(unsigned char *)S2)) {
			break;
		}
		S1 = S1 + 1;
		S2 = S2 + 1;
	}
	return (*(unsigned char *)S1 - *(unsigned char *)S2);
};

int strlength(const char *s) {
	int a = 0;
	for (const char *i = s;; i++) {
		if (*i == '\0') {
			break;
		}
		a++;
	}
	return a;
};

void strncopy(char *s1, const char *s2, int N) {
	const char *S2 = s2;
	for (int i = 0; *S2 != 0  && i < N; i++) {
		*(unsigned char *)s1 = *(unsigned char *)S2;
		s1 = s1 + 1;
		S2 = S2 + 1;
	}
	*(unsigned char *)s1 = '\0';
};

int main(int argc, char *argv[]) {
//  check for four command line arguments
//  if not, print error message and bail out 
	if (argc != 4) {
		return 1;
	}
//  declare two streams: file1, file2
	fstream file1;
	fstream file2;

  if (strcompare(argv[1], "-copy") == 0) {
	    char f1[512];
		char f2[512];
//    open file1 for reading
		file1.open(argv[2], ios::in);
//    open file2 for writing
		file2.open(argv[3], ios::out);

	while (file1.getline(f1, 512)) {		
//    determine length of textline1
		int x = strlength(f1);
//	  copy textline1 to textline2
		strncopy(f2, f1, x);
//	  write textline2 + newline to file2 
		file2.write(f2, x);
		file2.put('\n');
    }

  } else if (strcompare(argv[1], "-compare") == 0) {
//    open file1 for reading
		file1.open(argv[2], ios::in);
//    open file2 for reading
		file2.open(argv[3], ios::in);

		char f1[512];
		char f2[512];
//    for loop keeping track of linenumber
	for (int i = 1; i > 0; i++) {
//	  read textline1 from file1
		file1.getline(f1, 512);
//	  read textline2 from file2
		file2.getline(f2, 512);

	  if (file1.eof() && file2.eof()) {
	    break;
	  }
	  if (file1.eof()) {
	    cout << "1: EOF" << endl;
	    break;
	  }
	  if (file2.eof()) {
	    cout << "2: EOF" << endl;
	    break;
	  }
	  if (strcompare(f1, f2) != 0) {
	    cout << "Files differ on line " << i << endl;
	    cout << "1: " << f1 << endl;
		cout << "2: " << f2 << endl;
		break;
	  }
	  f1[0] = '\0';
	  f2[0] = '\0';
    }
  }
//  close both files
  file1.close();
  file2.close();
}
