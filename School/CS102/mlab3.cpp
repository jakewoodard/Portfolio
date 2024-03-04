// Kenneth Woodard
// Dunn Section 3
// 9/3/19
// for lab 3
// TA: Angel

#include <iostream>
#include <string>

using namespace std;

int main () {

int Number;
int Value; 

   cout << "Enter an integer: ";
   cin >> Number;

Value=Number%2;

if (Value == 0) { 
   cout << "This is an even number!";
}
else {
   cout << "This is an odd number!";
}
   cout << "\n";

return 0;
}
