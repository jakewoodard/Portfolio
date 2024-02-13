#include <iostream>
using namespace std;

int main () { 
int YYYY1;
int DD1;
int MM1;
int YYYY2;
int DD2;
int MM2;
int YYYY;
int DD;
int MM;
int value1;
int value2;
int value;

cout << "Enter start date: ";
cin >> YYYY1 >> MM1 >> DD1;
cout << "Enter end date: ";
cin >> YYYY2 >> MM2 >> DD2;
cout << "Enter date: ";
cin >> YYYY >> MM >> DD;

value1 = (YYYY1 * 10000) + (MM1 * 100) + DD1;
value2 = (YYYY2 * 10000) + (MM2 * 100) + DD2;
value = (YYYY * 10000) + (MM * 100) + DD;

if (value1 <= value && value2 >= value) {
cout << "IN RANGE\n";
}
else {
cout << "OUT OF RANGE\n";
}
return 0;
}
