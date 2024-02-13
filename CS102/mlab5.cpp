// Kenneth Woodard
// 09/18/19
// For Lab 5
// Dunn - Section 3
// TA: Angel


#include <iostream>
#include <random>
#include <limits>
#include <chrono>
using namespace std;

int main () {
  // variable 
  int seed;
  int array[25];
  int sum;
  int i;
  cout << "Enter a seed: ";
  cin >> seed;
   
  default_random_engine rng(seed);
  uniform_int_distribution<int> dist(-10, 10);
  
  sum = 0;

  for (i = 0; i < 25; i++) {
  array[i] = dist(rng); 
  }
  for (i = 0; i < 25; i++) {
  sum = sum + array[i];
  }
cout << "Sum of array = " << sum << "." << endl;
return 0;
}
