#include <iostream>
#include <string>
using namespace std;

main () {
	int numAddress;
	string avenue;
	string address;
	int lastmonthvalue;
	int currentmonthvalue;

	cout << "Enter the property street number, name, and type: ";
	cin >> numAddress >> address >> avenue;
    cout << "Last month's property value: ";
	cin >> lastmonthvalue;
    cout << "This month's property value: ";
    cin >> currentmonthvalue;
    cout << endl;
// output all info

    cout << "This building is located at " << numAddress << " " << address << " "  << avenue << ".\n";
	cout << "Last month, the value was " << lastmonthvalue << ".\n";
	cout << "This  month, the value was " << currentmonthvalue << ".\n";
	


	return 0;
}
