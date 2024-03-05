#include <iostream>
#include <cstdlib>
#include <utility>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

bool hasZero(int array[]) {
    for (int i = 0; i < 9; ++i) {
        if (array[i] == 0) {
            return true; // Found a zero value
        }
    }
    return false; // No zero values found
}

int getRandomNumber() {
    // Create a random number generator engine
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    // Define the distribution for integers between 1 and 9 (inclusive)
    std::uniform_int_distribution<> distr(1, 9);

    // Generate and return a random number
    return distr(eng);
}

int getRandomNumber2() {
    // Create a random number generator engine
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    // Define the distribution for integers between 1 and 9 (inclusive)
    std::uniform_int_distribution<> distr(30, 51);

    // Generate and return a random number
    return distr(eng);
}

void printBoard(int board[9][9]) {
	cout << "\n";
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			cout << " ";
			if (board[i][j] != 0 ) { cout << board[i][j]; }
			else { cout << ' '; }
			cout << " ";
			if (j % 3 == 2 && j != 8) cout << "|";
		}
		cout << "\n";
		if (i % 3 == 2 && i != 8) cout << "---------------------------\n";
	}
	return;
}

bool checkNum(int board[9][9], int row, int col, int num) {
	for (int i = 0; i < 9; i++) {
		if (board[row][i] == num || board[i][col] == num) return false;
	}
	
	// check particular 3 by 3 grid
	int startRow = row - row % 3;
    	int startCol = col - col % 3;

    	for (int i = 0; i < 3; ++i) {
        	for (int j = 0; j < 3; ++j) {
			if (board[i + startRow][j + startCol] == num) return false;
        	}
    	}

	return true;
}

// check if there is still an empty space
bool emptySpace(int board[9][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j] == 0) return true;
		}
	}
	return false;
}

std::pair<int, int> getEmptySpace(int board[9][9]) {
	for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        if (board[i][j] == 0) {
				int first = i;
				int second = j;
				return std::make_pair(first, second);
                	}
		}
        }
	int first = 10;
	int second = 10;
	return std::make_pair(first, second);
}

bool fillBoard(int board[9][9]) {
	int arr[9] = {0};

	while (emptySpace(board)) {
		std::pair<int, int> result = getEmptySpace(board);
		int row = result.first;
		int col = result.second;

		if (row == 8 && col == 8) {
			for (int i = 0; i < 9; i++) {
				if (checkNum(board, row, col, i+1)) {
					board[8][8] = i+1;
					return true;
				}
			}
		}
	
		// If no empty space left, the board is filled
		if (row == 10 && col == 10) {
			return true;
		}

		int num = getRandomNumber();
		arr[num-1] = 1;
		if (checkNum(board, row, col, num)) {
			board[row][col] = num;
			if (fillBoard(board)) return true;
			board[row][col] = 0;
		}
		if (!hasZero(arr)) return false;
	}
	return false;
}

void blankSpaces(int board[9][9], int numSpaces) {
	// if numSpaces is odd, remove middle space
	if (numSpaces % 2 == 1) {
		board[4][4] = 0;
		numSpaces--;
	}

	for (int i = 0; i < numSpaces; i++) {
		
		// generate random spaces to remove
		int row = getRandomNumber() - 1;
		int col = getRandomNumber() - 1;
		int row2;
		int col2;
		// calculate mirror of those spaces
		if ((row - 4) >= 0) {
			row2 = 4 - abs(row - 4);
		} else {
			row2 = 4 + abs(row - 4);
		}
		
		if ((col - 4) >= 0) {
                        col2 = 4 - abs(col - 4);
                } else {
              		col2 = 4 + abs(col - 4);
                }

		board[row][col] = 0;
		board[row2][col2] = 0;
		
		// increment again for 
		i++;
	}
}
	
int main() {

	// answer key board "filled" & player board
	int filled[9][9] = {0};
	int board[9][9];

	// create sudoku puzzle
	cout << "\nCreating sudoku board.\n";
	fillBoard(filled);
	
	// copy to player board and remove random amount of spaces
	int numSpaces = getRandomNumber2();
	cout << "\nRemoving ";
	cout << numSpaces;
	cout << " spaces. Good luck. \n\n";

	// copy board to player board
	for (int i = 0; i < 9; ++i) {
        	for (int j = 0; j < 9; ++j) {
            		board[i][j] = filled[i][j];
        	}
    	}

	// ask to meet you where u been?
	blankSpaces(board, numSpaces);

	printBoard(board);	
	return 0;
}
