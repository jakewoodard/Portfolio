// Exam 2 Study Practice

istringstream - Turns a string into an integer
ostringstream

What is a c-style string?

char[] - array of characters

*char[] - makes the array 2 dimensional

what does it mean when a parameter is constant?

The function cannot change its value. Used for reading the value only

./my_very_fun_program woohoo! -- what is the argc?

argc = total number of arguments(int) = 2
nameoffile = 1
2ndinput = 2

./my_prog 4 hi -- what is the argc?
argc=3

argv[] is an array of character arrays

ex - "Hello World" = {'H', 'E', 'L', 'L', 'O', '\0'} 

by default arrays are passed by reference, you do not need the AMPERSAND

literal vs const

void doubler(double &input) {
input *= 2;
return;
}

double x = 2;
doubler(x);
so x = 4;

but...
x
doubler(4);

