#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
using namespace std;

#include<iostream>
using namespace std;
struct B {
    B() : x(3) { }
    B(int x) : x(x) { }
    int x;
};

int main() {
    B* b = new B(7);
    cout << b->x;
    delete[] b;
}
