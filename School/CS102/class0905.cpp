// notes from class Sept, 5
// Kenneth Woodard

#include <iostream>
using namespace std;

int main () {

char choice;
string pet_type;

  cout << "Choose the pet type that you have: \n " << "(1) cat, (2) dog, (3) reptile, (4) other "; 
  cin >> choice;

switch (choice) {
  case '1':
    cout << "Cat's are annoying. Too bad!\n";
    break;
  case '2':
    cout << "Dog's are the best. Congrats.\n";
    break;
  case '3': 
    cout << "You slime!\n";
    break;
  case '4':
    cout << "What type of pet do you have?\n";
    cin >> pet_type;
    cout << "Wow! You have a " << pet_type << "!\n";
  default:
    cout << "See you never!\n";
    break;
}

return 0;
}
