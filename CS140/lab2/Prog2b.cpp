#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
	                    "8", "9", "10", "Jack", "Queen", "King" };
const string suit[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

class list {
	struct node { 
		node(int n = 0);
		int data;
		node *next;
	};

	public:
		list();   // constructor
		~list();  // destructor
		
		bool empty() const { return N == 0; }
		bool insert(const int &);
		void erase(int);
		
	private:
		node *head;
		int N;

	friend ostream & operator<<(ostream &, const list &);
};

list::node::node(int j) {
	data = j;
	next = NULL;
}

// constructor
list::list() { head = new node; N = 0; }

// destructor
list::~list() {
	while (!empty()) {
		erase(0);
	}
		delete head;
}

// insert function
bool list::insert(const int &j) {
	struct node *previous = this -> head;
	
	while (previous -> next) {
		if (previous -> next -> data == j) {
			struct node *p = previous -> next ->  next;
			delete previous -> next;
			previous -> next = p;
			return false;
		}

		if (previous -> next -> data > j) break;
		previous = previous -> next;
	}
	
	struct node *p = new struct node(j);
	p -> next = previous -> next;
	previous -> next = p;
	return true;
}

// erase function for destructor
void list::erase(int i) {
	node *p;
	
	if (i == -1) {
	  p = head; 
	}
	else {	
		node *p = head->next;   
		while (i--) { 
			p = p->next;
			if (i == 0) break;
		}
	}
	node *pp = p;  
  
	p = pp->next;  
	pp->next = p->next;
	
	N--;
	delete p;
}

// friend of class to print out results of game
ostream & operator<<(ostream & out, const list & k) {
	list::node *p = k.head -> next;	
	while (p != NULL) {
		out << " " << face[p -> data];
		p = p -> next;
	}
	return out;
}

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
	
	int w;				// int variable that keeps track of which suit
						// stops the game
	bool done = false;	// bool variable that keeps track of whether
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
	
	// declare array of lists to keep track
	// of cards that have been drawn
	list drawn[4];

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
						if (drawn[b].insert(a)) {
							table[b][a] += 1;
						}
						else {
							table[b][a] -= 1;
						}
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
		if (done == true) break;
	}

	// output formatted stats by suit
	for (int i = 0; i < 4; i++) {
		cout << setw(8) << setfill(' ')
			 << right << suit[i] << " :";
		cout << drawn[i];
		if (w == i) {
			cout << " **";
		}
		cout << "\n";
	}
return 0;
}
