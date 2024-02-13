// Kenneth Woodard
// December 4, 2019
// Section 3 - Dunn
// TA: Angel
// Lab 11 (Lab 7 Re-do)
// This program plays a card game with mulitple users. The program asks how many users and deals cards to those users. 

#include <string>
#include <iostream>
#include <array>
#include <iomanip>
#include <chrono>
#include <random>
using namespace std;

const string VALUES[] = {
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",   // 9D is the 21th element, deck[20]
    "10",
    "J",
    "Q",
    "K",
    "A",   // AH is the 13th element, deck[12]
};
const int NUM_VALUES = 13;

const string SUITS[] = {
    "H",
    "D",
    "S",
    "C",
};
const int NUM_SUITS = 4;

// functions to get card based on randomly generated index
string get_card_value(int index) {
    return VALUES [index % NUM_VALUES];
}

string get_card_suit(int index) {
    return SUITS [index / NUM_VALUES];
}

void initialize_deck(int cards[]) {
    for (int i = 0; i <= 51; i++) {
        cards[i] = 0;
    }
}

// function to return random index between 0 and 51
int get_random_index(int min, int max, default_random_engine &rng) {
    uniform_int_distribution<int> rand_int(min, max);
    int random_num = rand_int(rng);
    return random_num;
}

// function to deal cards to player and dealer
int deal_card(int min, int max, int num, int cards[], default_random_engine &rng) {
    int index;
    do {
        index = get_random_index(min, max, rng);
    } while (cards[index] != 0);
    cards[index] = num;
    return index;
}

// function to output cards to screen
void print_card(int num, int cards[]) {
    for (int i = 0; i <= 51; i++) {
        if (cards[i] == num) {
            cout << left << get_card_value(i) << get_card_suit(i) << " ";
        }
    }
}

int score_hand(int num, int cards[]) {
    int total_score = 0;
    for (int i = 0; i < 52; i++) {
        if (cards[i] == num) {
            int card_score = 0;
            if (i % 13 == 12) {
                card_score = 1;
            }
            else if (i % 13 <= 11 && i % 13 >= 8) {
                card_score = 10;
            }
            else {
                card_score = i % 13 + 2;
            }
            
            total_score += card_score;
        }
    }
    return total_score;
}

int main () {
    
    // dealer and user each start with 2 cards
    int dealer_score = 0;
    int player_score = 0;
    int num_players;
    
    // arrays to store deck of cards AND dealer/player ID "num"
    int deck[52];
    initialize_deck(deck);
    
    // max and min for random number generator
    int min = 0;
    int max = 51;
    
    // create random number generator based on time
    unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    
    // ask user for number of players
    cout << "How many players? ";
    cin >> num_players;
    
    // deal first 2 random indexes to each player using for-loop
    for (int i = 1; i <= num_players; i++) {
        deal_card(min, max, i, deck, rng);
        deal_card(min, max, i, deck, rng);
    }
   
    for (int i = 1; i <= num_players; i++) {
        cout << "Player " << i << " has cards: ";
        print_card(i, deck);
        cout << setw(10) << right << "(" << score_hand(i, deck) << ")" << endl;
    }
    return 0;
}
