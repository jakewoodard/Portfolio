#include <cctype> 
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

enum format  { BINARY, DECIMAL, HEXADEC };

/*------------ FUNCTIONS IN LIBNC ------------*/

void printusage();

void dec2bin(string &, string &);
void bin2dec(string &, string &);
void bin2hex(string &, string &);
void hex2bin(string &, string &);

void processDECIMAL(string &, string &, format);
void processBINARY(string &, string &, format);
void processHEXADEC(string &, string &, format);

/*----------- YOUR IMPLEMENTATIONS -----------*/

void dec2bin(string &input, string &output) {
int dec;
stringstream fella(input);
fella >> dec;
int i = 1;
int value = 0;
        while (dec > 0) {
	    //    value *= 10;
		      int remainder = dec % 2;
			  value += remainder * i;
			  dec /= 2;
			  i *=10;
	    }
    stringstream ss;
    ss << value;
	output = ss.str();
}

void bin2dec(string &input, string &output) {
	int bin;
	stringstream fella(input);
	fella >> bin;
	int value = 0;
	int i = 0;
	while (bin > 0) {
	//	value *= 2;
		int remainder = bin % 10;
		value += remainder * pow(2,i);
		bin /= 10;	 
		++i;
	}
stringstream ss;
ss << value;
output = ss.str();
}

// void bin2hex(string &, string &) { optional }
// void hex2bin(string &, string &) { optional }

/*----------- USER INTERFACE CODE ------------*/

int main(int argc, char *argv[]) {
  string mode;
  string input;
  string output;

  format imode = DECIMAL;
  format omode = BINARY;

  while (1) {
    cout << "enter> ";
    cin >> mode;

	if (cin.eof())
	  break;

	if (mode.compare("dec2bin") == 0) {
      imode = DECIMAL;
      omode = BINARY;
    } else 
	if (mode.compare("dec2hex") == 0) {
      imode = DECIMAL;
      omode = HEXADEC;
    } else 
	if (mode.compare("bin2dec") == 0) {
      imode = BINARY;
      omode = DECIMAL;
    } else 
	if (mode.compare("bin2hex") == 0) {
      imode = BINARY;
      omode = HEXADEC;
    } else 
	if (mode.compare("hex2bin") == 0) {
      imode = HEXADEC;
      omode = BINARY;
    } else 
	if (mode.compare("hex2dec") == 0) {
      imode = HEXADEC;
      omode = DECIMAL;
    } else {
	  cout << mode << " unknown\n";
	  continue;
	}

    cin >> input;

    switch (imode) {
    case DECIMAL:
      processDECIMAL(input, output, omode);
      break;
	case BINARY:
      processBINARY(input, output, omode);
      break;
    case HEXADEC:
      processHEXADEC(input, output, omode);
      break;
    }

	cout << output << "\n";
  }
}
