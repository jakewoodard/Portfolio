// Kenneth Woodard
// Dunn - Section 3
// TA: Angel
// Lab 8
// November 9th, 2019
// This program plays a game of blackjack with one player (the user). The goal is to get to 21 points without going over.
// TA: Syd (Office Hour Help)

#include <iostream> 
#include <string>
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
            cout << get_card_value(i) << get_card_suit(i) << " ";
        }
    }
}

// function to score hand
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
    
    // arrays to store deck of cards AND dealer/player ID "num"
    
    int deck[52];
    int player_num = 1; // player ID is one
    int dealer_num = 2; // dealer ID is two
    initialize_deck(deck);
    
    // max and min for random number generator
    int min = 0;
    int max = 51;

    // create random number generator based on time
    unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    // deal first 2 random indexes to dealer and player
    deal_card(min, max, dealer_num, deck, rng);
    deal_card(min, max, dealer_num, deck, rng);
    
    deal_card(min, max, player_num, deck, rng);
    deal_card(min, max, player_num, deck, rng);
    
    // output cards and score to screen for user
    cout << "Dealer has cards: ";
    print_card(dealer_num, deck);
    cout << setw(6);
    cout << "(" << score_hand(dealer_num, deck) << ")" << endl;
    dealer_score = score_hand(dealer_num, deck); // assign score
    
    cout << "You have cards  : ";
    print_card(player_num, deck);
    cout << setw(6);
    cout << "(" << score_hand(player_num, deck) << ")" << endl;
    player_score = score_hand(player_num, deck);
 
    // ask user hit or stand
    string decision;
    do {
    cout << "Hit or stand    ? ";
    cin >> decision;
        if (decision != "stand" && decision != "hit") {
            cout << "Invalid response." << endl;
        }
    cout << endl;
    
        
        // if statement for decision = "hit"
        if (decision == "hit") {
            
            // deal_card(player_cards[], num_player_cards);
            deal_card(min, max, player_num, deck, rng);
            cout << "Dealer has cards: ";
            print_card(dealer_num, deck);
            cout << setw(6);
            cout << "(" << score_hand(dealer_num, deck) << ")" << endl;
            cout << "You have cards  : ";
            print_card(player_num, deck);
            cout << setw(6);
            cout << "(" << score_hand(player_num, deck) << ")" << endl;
        
        }
        player_score = score_hand(player_num, deck);
    } while (decision != "stand" && player_score < 22);
    
    // if player stands and if dealer's score is 17 or lower, then the dealer will hit as needed
    if (player_score < 22) {
    while (dealer_score < 18) {
        
        cout << "Dealer hits     : ";
        deal_card(min, max, dealer_num, deck, rng);
        print_card(dealer_num, deck);
        cout << setw(6);
        cout << "(" << score_hand(dealer_num, deck) << ")" << endl;
        dealer_score = score_hand(dealer_num, deck);
    }
    
    if (dealer_score > 17 && dealer_score < 22) {
        cout << "Dealer stands   : ";
        print_card(dealer_num, deck);
        cout << setw(6);
        cout << "(" << score_hand(dealer_num, deck) << ")" << endl;
        dealer_score = score_hand(dealer_num, deck);
    }
    }
    
    // output final results
    if (player_score > 21) {
        cout << "Player busts, dealer wins!" << endl;
    }
    else if (dealer_score > 21) {
        cout << "Dealer busts, player wins!" << endl;
    }
    else if (player_score > dealer_score) {
        cout << "Player wins!" << endl;
    }
    else if (dealer_score > player_score) {
        cout << "Dealer wins!" << endl;
    }
    else {
        cout << "Player and dealer draw." << endl;
    }

    return 0;
}

