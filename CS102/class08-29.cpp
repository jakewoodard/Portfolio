#include <iostream>
using namespace std;
// playing around w operators

int main()
{
  int num1, num2;
  const double PI = 3.14;
  double radius;

  cout << "Please input two real numbers: ";
  cin >> num1 >> num2;

  cout << num1 << "+" << num2 << "=" << num1 + num2 << '\n';
  cout << "--num1: " << --num1 << '\n';
  cout << "num1--: " << num1-- << '\n';
  cout << "What is the radius of the circle? ";
  cin >> radius;
  cout << "The area of the circle is: ";
  
return 0;
  
} 
