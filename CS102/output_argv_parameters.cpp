// Output all argv parameters
#include <iostream>
using namespace std;
// Functions
int main(int argc, char *argv[])
{ 
 for (int i = 0; i < argc; ++i) {
    cout << argv[i] << ' ';
  } 
 cout << '\n';
 
 return 0;
}
