// Kenneth Woodard
// September 25, 2019
// Dunn - Section 3
// Lab 5
// TA: Angel
// This program simulates a slot machine, starting the user off with $1000. The user places a bet before every spin. 

#include <iostream>
#include <vector>
#include <array>
#include <limits>
#include <random>
using namespace std;

int main () {
  vector<double> Winnings;
  vector<double> Bets;
  double bet;
  double winning; 
  double amount;
  double maxbet;
  double minbet;
  double maxwinning;
  double minwinning;
  int reel[3];
  int seed;
  int i;
  int p; // times played
  int j; 

// initial amount
amount = 1000;
p = 0;
maxbet = 0;
minbet = 100000;
maxwinning = 0;
minwinning = 10000000;

// get input for seed
do {
  cout << "Input a seed: ";
  cin >> seed;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
} while (seed == 0);

// create random number generator
default_random_engine rng(seed);
uniform_int_distribution<int>dist(2,7);

do {
  cout << "Your money: $" << amount << endl;
  cout << "Place a bet: $";
  cin >> bet;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

if (bet == 0 && p == 0) {
  cout << "You walked away without playing.";
}
   
// output slot machine results
else {
// run slot machine
    for (i = 0; i < 3; i++) {
        reel[i] = dist(rng);
    }
// output slot machine results
cout << " " << reel[0] << " " << reel[1] << " " << reel [2] << endl;
}



// if statements for slot results
    


    if (bet == 0 && p > 0) {
     
    // create for loop to output all info on wagers and winnings for user
        for (j = 0; j < p; j++) {
        if (Winnings.at(j) > 0) {
        cout << "You wagered $" << Bets.at(j) << " and won $" << Winnings.at(j) << ".";
        }
        else {
        cout << "You wagered $" << Bets.at(j) << " and won $0.";
        }
        cout << endl;
        }

	// Output maxes and mins
	cout << endl;
	cout << "Your smallest wager was $" << minbet << ".\n";
	cout << "Your largest wager was $" << maxbet << ".\n";
	cout << "Your smallest winnings was $" << minwinning << ".\n";
	cout << "Your largest winnings was $" << maxwinning << ".\n";
        }

    else if ((reel[0] == reel[1]) && (reel[1] == reel[2]) && (reel[1] == 7)) { 
	winning = bet * 10;
	// store information 
	Winnings.push_back(winning);
	Bets.push_back(bet);
        cout << "You won $" << winning << "!\n";
    }
    else if ((reel[0] == reel[1]) && (reel[1] == reel[2]) && (reel[1] != 7)) {
	winning = bet * 5;
	Winnings.push_back(winning);
        Bets.push_back(bet);
        cout << "You won $" << winning << "!\n";
    }
    else if ((reel[0] == reel[1]) || (reel[0] == reel[2]) || (reel[1] == reel[2])) {
	winning = bet * 2;
	Winnings.push_back(winning);
        Bets.push_back(bet);
        cout << "You won $" << winning << "!\n";
    }
    else if ((reel[0] != reel[1]) && (reel[0] != reel[2]) && (reel[1] != reel[2])) {
        winning = bet *-1;
	Winnings.push_back(winning);
        Bets.push_back(bet);
        cout << "You didn't win.\n";
    }

// edit amount based on winnings
amount = amount + winning;

// keep track of total times played
p++;

// record maxes and mins
if (maxbet < bet) {
maxbet = bet;
}
if (bet < minbet) {
minbet = bet;
}
if (maxwinning < winning) {
maxwinning = winning;
}
if (winning > 0 && minwinning > winning) {
minwinning = winning;
}

} while (bet > 0 && amount > 0);
cout << endl;

if (amount <= 0) {
cout << "You are out of money!\n";
for (j = 0; j < p; j++) {
        if (Winnings.at(j) > 0) {
        cout << "You wagered $" << Bets.at(j) << " and won $" << Winnings.at(j) << ".";
        }
        else {
        cout << "You wagered $" << Bets.at(j) << " and won $0.";
        }
        cout << endl;
        }

        // Output maxes and mins
        cout << endl;
        cout << "Your smallest wager was $" << minbet << ".\n";
        cout << "Your largest wager was $" << maxbet << ".\n";
        cout << "Your smallest winnings was $" << minwinning << ".\n";
        cout << "Your largest winnings was $" << maxwinning << ".\n";
}

return 0;
}
