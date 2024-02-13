// Kenneth Woodard
// 09/04/19
// Dunn: Section 3
// lab 3
// TA: Angel
// User inputs an Interstate Number and the program describes the interstate for the user 

#include <iostream>
using namespace std;

int main () {

  int Interstate;

  cout << "Enter an interstate number: ";
  cin >> Interstate;

     // Make if statements to separate odd and even numbers
     // As well as two digit and three digit numbers
     // Use modulo to make 100, 200, etc invalid as well as 0 or any number over 1000

  
  if (Interstate <= 99 && Interstate > 0) {
    cout << "I-" << Interstate << " is a primary interstate,";
  }
  else if (100 < Interstate && Interstate < 1000 && (Interstate % 100 !=0)) {
    cout << "I-" << Interstate << " is a auxiliary interstate, serving I-" << Interstate % 100 << ",";
  }
  else {
    cout << Interstate << " is not a valid interstate number.\n";
  }
  if (Interstate <= 99 && Interstate > 0 && (Interstate % 2 == 0)) {
    cout << " going east/west.\n";
  }
  else if (Interstate <= 99 && Interstate > 0 && (Interstate % 2 == 1)) {
    cout << " going north/south.\n";
  }
  else if (100 < Interstate && Interstate < 1000 && (Interstate % 2 == 0) && (Interstate % 100 != 0)) {
    cout << " going east/west.\n";
  }
  else if (100 < Interstate && Interstate < 1000 && (Interstate % 2 == 1) && (Interstate % 100 != 0)) {
    cout << " going north/south.\n";
  }
return 0;
}   
