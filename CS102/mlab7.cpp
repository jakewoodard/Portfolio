#include <iostream>
#include <string>
#include <array>
using namespace std;

const string VALUES[] = {
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"J",
	"Q",
	"K",
	"A"
};
const int NUM_VALUES = 13;

const string SUITS[] = {
    "Hearts",
    "Diamonds",
    "Spades",
    "Clubs"
};
const int NUM_SUITS = 4;

string get_card_value(int index) 
{
    return VALUES [index % NUM_VALUES];
}

string get_card_suit(int index)
{
    index /= NUM_VALUES;
    return SUITS[index];
}



int main() {
    int cards[52];
    
    int value;
    cout << "Enter an index: ";
    cin >> value;
    
    cout << "That is the " << get_card_value(value) << " of " << get_card_suit(value) << ".\n";

return 0;
}
