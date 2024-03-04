#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
using namespace std; 

// function to print board
void printBoard(char progress[3][3]) {
	cout << "\n\n";
	cout << " A   B   C\n\n";
	cout << " ";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int x = i + 1; 
			cout << progress[i][j];
			if (j != 2) { cout << " | ";
			} else { cout << " "; cout << x; cout << "\n"; }
		}
		if (i != 2) cout << "----------\n ";
	}
	cout << "\n\n";
	return;
}

// function to check if there are 3 in a row
void check3inrow(char (*progress)[3]) {
        // did X or O win?
        char winner = ' ';

        // check rows first
        if (progress[0][0] == progress[1][0] && progress[0][0] == progress [2][0]) { winner = progress[0][0]; }

        if (progress[0][1] == progress[1][1] && progress[0][1] == progress [2][1]) { winner = progress[0][1]; }

        if (progress[0][2] == progress[1][2] && progress[0][2] == progress [2][2]) { winner = progress[0][2]; }

        // check columns second
        if (progress[0][0] == progress[0][1] && progress[0][0] == progress [0][2]) { winner = progress[0][0]; }

        if (progress[1][0] == progress[1][1] && progress[1][0] == progress [1][2]) { winner = progress[1][0]; }

        if (progress[2][0] == progress[2][1] && progress[2][0] == progress [2][2]) { winner = progress[2][0]; }

        // check diagonals third
        if (progress[0][0] == progress[1][1] && progress[0][0] == progress [2][2]) { winner = progress[0][0]; }

        if (progress[2][0] == progress[1][1] && progress[2][0] == progress [0][2]) { winner = progress[1][1]; }

        // check who won
        if (winner == 'X') {
                cout << "You bested me...\n.";
                exit(0);
        }
        if (winner == 'O') {
                cout << "Suck it. You're garbage.\n";
                exit(0);
        }
}

char check3(char(*progress)[3]) {
	// can someone win?
        char winner = ' ';

        // check rows first
        if (progress[0][0] == progress[1][0] && progress[0][0] == progress [2][0]) { winner = progress[0][0]; }

        if (progress[0][1] == progress[1][1] && progress[0][1] == progress [2][1]) { winner = progress[0][1]; }

        if (progress[0][2] == progress[1][2] && progress[0][2] == progress [2][2]) { winner = progress[0][2]; }

        // check columns second
        if (progress[0][0] == progress[0][1] && progress[0][0] == progress [0][2]) { winner = progress[0][0]; }

        if (progress[1][0] == progress[1][1] && progress[1][0] == progress [1][2]) { winner = progress[1][0]; }

        if (progress[2][0] == progress[2][1] && progress[2][0] == progress [2][2]) { winner = progress[2][0]; }

        // check diagonals third
        if (progress[0][0] == progress[1][1] && progress[0][0] == progress [2][2]) { winner = progress[0][0]; }

        if (progress[2][0] == progress[1][1] && progress[2][0] == progress [0][2]) { winner = progress[1][1]; }

	return winner;
}

// function for player to place X
void playerTurn(char (*progress)[3]) {
	while (1) {
		string X = "";
		while (X != "A1" && X != "A2" && X != "A3" && X != "B1" && X != "B2" && X != "B3" && X != "C1" && X != "C2" && X != "C3") {
			cout << "Enter move coordinate. (A1, C2, etc.):  ";
			cin >> X;
		}
		if (progress[int(X[1]-'1')][int(X[0]-'A')] != 'O' && progress[int(X[1]-'1')][int(X[0]-'A')] != 'X') {
			progress[int(X[1]-'1')][int(X[0]-'A')] = 'X';
			printBoard(progress);
			check3inrow(progress);
			return;
		} else {
			cout << "Space already filled.\n";
		}
	}
}

// function for cpu to place O
void EasyTurn(char (*progress)[3]) {
	usleep(250000);
       	srand(time(nullptr)); // Seed the random number generator
	cout << "My turn.\n";
	usleep(500000);
	while (1) {
		int O1 = rand() % 3;
		int O2 = rand() % 3;
		if (progress[O1][O2] != 'X' && progress[O1][O2] != 'O') {
			progress[O1][O2] = 'O';
			printBoard(progress);
			check3inrow(progress);
			return;
		}
	}
}

void HardTurn(char (*progress)[3]) {
	usleep(250000);
	cout << "My turn.\n";
	usleep(500000);

	// check for winning move
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (progress[i][j] != 'X' && progress[i][j] != 'O') {
				progress[i][j] = 'O';
				if (check3(progress) == 'O') {
					printBoard(progress); 
					check3inrow(progress);
				} else {
					progress[i][j] = ' ';
				}
			}
		}
	}

        // check for blocking move
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                        if (progress[i][j] != 'X' && progress[i][j] != 'O') {
                                progress[i][j] = 'X';
                                if (check3(progress) == 'X') {
					progress[i][j] = 'O';
                                        printBoard(progress);
					return;
                                } else {
                                        progress[i][j] = ' ';
                                }
                        }
                }
        }

	// else place randomly
	srand(time(nullptr)); // Seed the random number generator
	while (1) {
		int O1 = rand() % 3;
               	int O2 = rand() % 3;
                if (progress[O1][O2] != 'X' && progress[O1][O2] != 'O') {
                        progress[O1][O2] = 'O';
                        printBoard(progress);
                        check3inrow(progress);
                        return;
                }
	}
}

// main program
int main() {

	cout << "Want to play Tic-Tac-Toe?\n";
	// does user want to play vs friend or cpu
	string difficulty = " ";
	cout << "Easy or Hard? (E/H)\n";
	cin >> difficulty;
	while (difficulty != "E" && difficulty != "H" && difficulty != "e" && difficulty != "h") {
		cout << "Answer must be either E or H.\n";
		cin >> difficulty;
	}
	cout << "\n";

	// hard difficulty
	if (difficulty == "H" || difficulty == "h") {
		cout << "You've got no shot.\n\n";
		usleep(500000);
		cout << "You go first; therefore, you are X.\n";
                usleep(500000);
		char board[3][3] = {
                        {' ', ' ', ' '},
                        {' ', ' ', ' '},
                        {' ', ' ', ' '}
                };

		// print empty board and play
		printBoard(board);
		playerTurn(board); // 1
                HardTurn(board);   // 2
                playerTurn(board); // 3
                HardTurn(board);   // 4
                playerTurn(board); // 5
                HardTurn(board);   // 6
                playerTurn(board); // 7
                HardTurn(board);   // 8
                playerTurn(board); // 9
		// tie
		cout << "Tie! Which means you couldn't beat me. HAH!\n";
	} else {
	// easy difficulty
		cout << "You're scared.\n";
		usleep(500000);
		cout << "You go first; therefore, you are X.\n";
		usleep(500000);
		char board[3][3] = {
			{' ', ' ', ' '},
			{' ', ' ', ' '},
			{' ', ' ', ' '}
		};

		// print empty board and play
		printBoard(board);
		playerTurn(board); // 1
		EasyTurn(board);   // 2
		playerTurn(board); // 3
                EasyTurn(board);   // 4
		playerTurn(board); // 5
                EasyTurn(board);   // 6
		playerTurn(board); // 7
                EasyTurn(board);   // 8
		playerTurn(board); // 9
		
		// tie
		cout << "Tie! Which means you couldn't beat me. HAH!\n";
	}

	// End of Program
	return 0;
}

