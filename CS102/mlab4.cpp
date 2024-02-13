// Kenneth James Woodard
// September 8, 2019
// CS102
// Dunn - Section 3
// TA: Angel
// This code keeps track of how much money is in someone's account.

#include <iostream>
#include <string>
using namespace std;

int main () {
  float totalBalance;
  char choice;
  float amount; // variable for withdrawal and deposits

  cout << "How much money is in your account? $";
  cin >> totalBalance;
  
// Create a loop that ends when the user decides to quit the program

do {
  cout << "What would you like to do?\n" << "(q = quit, b = view balance, w = withdraw, d = deposit)\n";
  cin >> choice;
  
// create switch system for different choices 

switch (choice) {
  case 'q':
    cout << "Goodbye!\n";
    break;
  case 'b':
    cout << "Your balance is $" << totalBalance << ".\n";
    break;
  case 'w':
    cout << "How much would you like to withdraw? $";
    cin >> amount;
       totalBalance = totalBalance - amount;
    cout << "Your balance is now $" << totalBalance << ".\n";
    break;
  case 'd':
    cout << "How much would you like to deposit? $";
    cin >> amount;
       totalBalance = totalBalance + amount;
    cout << "Your balance is now $" << totalBalance << ".\n";
    break;
}

} while (choice != 'q');
return 0;
}
