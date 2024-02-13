#include <cstdlib>
#include <iostream>
using namespace std;

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
                        "8", "9", "10", "Jack", "Queen", "King" }; 
const string suit[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

string random_card(bool verbose=false) {
	string card;

	card = face[ rand()%13 ];
	card += " of ";
	card += suit[ rand()%4 ];

	if (verbose)
	  cout << card << "\n";

	return card;
}

int main(int argc, char *argv[])
{
	bool verbose = false;
	int seedvalue = 0;
	int w; // int variable that keeps track of which suit
	       // stops the game
	bool done = false;
		   // bool variable that keeps track of whether 
		   // or not to end the game once condition met

	for (int i=1; i<argc; i++) {
	  string option = argv[i];
	  if (option.compare(0,6,"-seed=") == 0) {
	    seedvalue = atoi(&argv[i][6]);
	  } else if (option.compare("-verbose") == 0) {
	    verbose = true;
	  } else 
	    cout << "option " << argv[i] << " ignored\n";
	}

	srand(seedvalue);

	// declare a table[][] that can keep track of the
	// cards dealt for each suit -- initialize to 0
	int table[4][13] = {0};

	while (1) {
	    string card = random_card(verbose);
		
	  // reverse engineer card suit and face (value)
	  // update accordingly: table[suit][value]++
		string c = "";
		for (int a = 0; a < 13; a++) {
			c = face[a];
			if (c.compare(0, 1, card, 0, 1) == 0) {
				for (int b = 0; b < 4; b++) {
					string d = suit[b];
					c = face[a];
					c += " of ";
					c += suit[b];
					if (card.compare(c) == 0) {
						table[b][a] += 1;
					}
				}
			}
		}

      // break out of loop if stopping criteria met
		for (int h = 0; h < 4; h++) {
			int t = table[h][10];
			int u = table[h][11];
			int v = table[h][12];
			if ((t > 0) && (u > 0) && (v > 0)) {
				w = h;
				done = true;
			}
		}
		
		if (done == true) 
			break;

	}
	
	// print formatted table contents to stdout 
	cout << "   Clubs:";
	for (int j = 0; j < 13; j++) {
		cout << "   " << table[0][j];
	}
	if (w == 0) {
		cout << " **";
	}
	cout << "\n";
	
	cout << "Diamonds:";
    for (int j = 0; j < 13; j++) {
        cout << "   " << table[1][j];
	}
	if (w == 1) {
        cout << " **";
	}
	cout << "\n";

	cout << "  Hearts:";
	for (int j = 0; j < 13; j++) {
	    cout << "   " << table[2][j];
    }
	if (w == 2) {
        cout << " **";
    }
	cout << "\n";

	cout << "  Spades:";
	for (int j = 0; j < 13; j++) {
		cout << "   " << table[3][j];
    }
	if (w == 3) {
        cout << " **";
    }
	cout << "\n";

return 0;
}
