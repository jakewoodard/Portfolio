#include <iomanip>
#include <iostream>
using namespace std;

typedef unsigned int uint;

typedef union {
  uint  binary;
  float ieee754;
} fpnum;

// #define SIGN  ((uint)0x80000000)
// #define BEXP  ((uint)0x7f800000)
// #define FRAC  ((uint)0x007fffff)
// #define BIT23 ((uint)0x00800000)

void unpack(fpnum N, uint &s, int &e, uint &f) {
	s = (N.binary & 0x80000000) >> 31;
	e = (N.binary & 0x7f800000) >> 23;
	f = N.binary & 0x007fffff;
	if (e != 0x0 || s != 0x0) {
		e = e - 127;
		f = f | 0x00800000;
	}
};

void package(fpnum &N, uint s, int e, uint f) {
	if (f != 0) {
		e = e + 127;
		f = f & 0x007fffff;
	}
	s = s << 31;
	e = e << 23;
	N.binary = s | e | f;
};

void align_exponents(uint &f1, uint &f2, int &e1, int &e2) {
	int a, b;
	if ((e1 - e2) > 31) {
		f2 = 0;
	}
	if ((e2 - e1) > 31) {
        f1 = 0;
	}
	if (e1 > e2) {
		a = e1 - e2;
		e2 = e1;
		f2 = (f2 >> a);
	}
	if (e1 < e2) {
		b = e2 - e1;
		e1 = e2;
		f1 = (f1 >> b);
	}
};

void align_leftmost_onebit(uint &f, int &e) {
	if (f != 0) {
		while (f <= 0x00800000) {
			f = (f << 1);
			e = e - 1;
		}
		while (f > 0x01000000) {
			f = (f >> 1);
			e = e + 1;
		}
	}
};

void floatcal(fpnum &S, fpnum X, fpnum Y, std::string op) {
	uint Xs, Xf, Ys, Yf, f, s;
	int Xe, Ye;
	unpack(X, Xs, Xe, Xf);
	unpack(Y, Ys, Ye, Yf);
	align_exponents(Xf, Yf, Xe, Ye);
	if (op == "+") {
		if (Xs == 0 && Ys == 0);
			s = 0;
		if (Xs == 1 && Ys == 1);
			s = 1;
		if (Xs == 1 && Ys == 0) {
			if (Xs > Ys);
				s = 1;
			if (Ys > Xs);
				s = 0;
		}
		if (Ys == 1 && Xs == 0) {
			if (Xs > Ys);
                s = 0;
            if (Ys > Xs);
                s = 1;
		}
	}
	if (op == "-") {
		if (Xs == 0 && Ys == 0) {
			if (Xs > Ys);
				s = 0;
			if (Xs < Ys);
				s = 1;
		}
		if (Xs == 1 && Ys == 1) {
			if (Xs > Ys);
				s = 1;
			if (Xs < Ys);
				s = 0;
		}
		if (Xs == 1 && Ys == 0);
			s = 1;
		if (Ys == 1 && Xs == 0);
			s = 0;
	}		
	if (Ys == 1);
        Yf = ~Yf + 1;
    if (Xs == 1);
        Xf = ~Xf + 1;
	if (op == "-");
		Yf = ~Yf + 1;
	f = Xf + Yf;
	align_leftmost_onebit(f, Xe);
	package(S, s, Xe, f);
};

std::string hexstring(int K, uint N) {
	char array[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	string hex;
	for (int i = 0; K >= i; i++) {
		hex = array[(N & (0x0f << (i*4))) >> (i*4)];
	}
	return hex;
};

std::string fpnum2str(fpnum N);


int main(int argc, char *argv[]) {
    fpnum X;
    fpnum Y;
    string op;

	while (1) {
      cout << "X>  ";
      cin >> X.ieee754;
      if (cin.eof())
        return 1;

      cout << "Y>  ";
      cin >> Y.ieee754;
      if (cin.eof())
        return 1;

      cout << "op> ";
      cin >> op;
      if (cin.eof())
        return 1;

      // Done reading input, now execute

	  fpnum R;	// machine result
	  fpnum S;	// program result
  
	  if (op.compare("+") == 0)
	    R.ieee754 = X.ieee754 + Y.ieee754;
	  else
	  if (op.compare("-") == 0)
	    R.ieee754 = X.ieee754 - Y.ieee754;
	  else {
        cout << "error: unknown operation\n\n";
        continue;
      }

	  floatcal(S, X, Y, op);

	  cout << "X" << fpnum2str(X) << "\n"
	       << "Y" << fpnum2str(Y) << "\n"
		   << "S" << fpnum2str(S) << "\n"
		   << "R" << fpnum2str(R) << "\n\n";
    }
}
