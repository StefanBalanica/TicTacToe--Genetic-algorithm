#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cctype>

struct Individual {
	std::vector<int> genes;
	int fitness;
};

void initializeBoard(char board[3][3]) {
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			board[i][j] = ' ';
}

int isGameOver(char board[3][3], char symbol) {

	for (int i = 0; i < 3; ++i) {
		if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
			return 1;
		if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)
			return 1;
	}


	if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
		return 1;
	if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
		return 1;


	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (board[i][j] == ' ')
				return 0;

	return 2;
}

void printBoard(char board[3][3]) {
	std::cout << "------------\n";
	for (int i = 0; i < 3; ++i) {
		std::cout << "| ";
		for (int j = 0; j < 3; ++j) {
			std::cout << board[i][j] << " | ";
		}
		std::cout << "\n------------\n";
	}
	std::cout << '\n';
}

bool isValidMove(char board[3][3], int row, int col) {
	return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
}

void playerMove(char board[3][3], int currentGame) {
	int row, col;

	do {
		std::cout << "Introduceti coordonatele (rand si coloana) pentru mutarea dvs.\n(De exemplu -> 1 2): ";
		std::cin >> row >> col;
		--row;
		--col;
		if (!isValidMove(board, row, col)) std::cout << "Mutarea este gresita !\n";
	} while (!isValidMove(board, row, col));

	if (currentGame % 2 == 1) {
		board[row][col] = 'X';
	}
	else {
		board[row][col] = '0';
	}
}

int evaluateFitness(char board[3][3], const Individual& individual, char playerSymbol, char algorithmSymbol) {

	if (isGameOver(board, algorithmSymbol))
		return -10;
	else if (isGameOver(board, playerSymbol))
		return 10;
	else
		return 0;
}

void mutate(Individual& individual) {

	srand(static_cast<unsigned int>(time(nullptr)));
	int index = rand() % individual.genes.size();
	individual.genes[index] = rand() % 9;
}

void crossover(const Individual& parent1, const Individual& parent2, Individual& child) {

	srand(static_cast<unsigned int>(time(nullptr)));
	int crossoverPoint = rand() % (parent1.genes.size() - 1) + 1;

	std::copy(parent1.genes.begin(), parent1.genes.begin() + crossoverPoint, child.genes.begin());

	std::copy(parent2.genes.begin() + crossoverPoint, parent2.genes.end(), child.genes.begin() + crossoverPoint);
}

void geneticAlgorithmMove(char board[3][3], Individual& individual, char playerSymbol, char algorithmSymbol, int roundNumber) {

	individual.fitness = evaluateFitness(board, individual, playerSymbol, algorithmSymbol);

	individual.genes.resize(1);


	std::vector<Individual> mutations;
	for (int i = 0; i < individual.genes.size(); ++i) {
		Individual mutatedIndividual = individual;
		mutatedIndividual.genes[i] = rand() % 9;
		mutations.push_back(mutatedIndividual);
	}

	for (const auto& mutation : mutations) {
		if (evaluateFitness(board, mutation, playerSymbol, algorithmSymbol) > individual.fitness) {
			individual = mutation;
		}
	}

	if (individual.fitness >= evaluateFitness(board, individual, playerSymbol, algorithmSymbol))
		mutate(individual);

	int move = individual.genes[0];
	int row = move / 3;
	int col = move % 3;

	while (!isValidMove(board, row, col)) {
		move = rand() % 9;
		row = move / 3;
		col = move % 3;
	}

	board[row][col] = algorithmSymbol;
	std::cout << "Algoritmul a facut o mutare in runda " << roundNumber << ":\n\n";
}

bool askToContinue(int& playerScore, int& algorithmScore) {
	std::string response;
	std::cout << "Doriti sa continuati jocul? (DA/NU): ";
	std::cin >> response;
	for (char& c : response) {
		c = std::tolower(c);
	}

	if (response == "da") {
		return true;
	}
	else {
		std::cout << "Scor final:\n";
		std::cout << "Jucator: " << playerScore << " puncte\n";
		std::cout << "Algoritm: " << algorithmScore << " puncte\n";
		return false;
	}
}

int main() {
	char board[3][3];
	initializeBoard(board);

	int playerScore = 0;
	int algorithmScore = 0;
	int currentGame = 1;

	do {
		bool playerTurn = (currentGame % 2 == 1);

		if (currentGame % 2 == 1) {
			while (isGameOver(board, 'X') == 0 && isGameOver(board, 'O') == 0) {
				printBoard(board);

				if (playerTurn) {
					if (playerTurn && currentGame % 2 == 1) {
						playerMove(board, currentGame);
					}
					else {
						Individual individual;
						individual.genes.resize(1);
						geneticAlgorithmMove(board, individual, 'X', 'O', currentGame);
					}
				}
				else {
					if (!playerTurn && currentGame % 2 == 0) {
						playerMove(board, currentGame);
					}
					else {
						Individual individual;
						individual.genes.resize(1);
						geneticAlgorithmMove(board, individual, 'X', 'O', currentGame);
					}
				}

				playerTurn = !playerTurn;
			}
		}
		else {
			while (isGameOver(board, 'X') == 0 && isGameOver(board, 'O') == 0) {
				printBoard(board);

				if (playerTurn) {
					if (playerTurn && currentGame % 2 == 1) {
						Individual individual;
						individual.genes.resize(1);
						geneticAlgorithmMove(board, individual, 'O', 'X', currentGame);
					}
					else {
						playerMove(board, currentGame);
					}
				}
				else {
					if (!playerTurn && currentGame % 2 == 0) {
						Individual individual;
						individual.genes.resize(1);
						geneticAlgorithmMove(board, individual, 'O', 'X', currentGame);
					}
					else {
						playerMove(board, currentGame);
					}
				}

				playerTurn = !playerTurn;
			}
		}

		printBoard(board);

		if (isGameOver(board, 'X') == 2 && isGameOver(board, 'O') == 2) {
			std::cout << "Remiza!\n";
		}
		else if (isGameOver(board, 'X') == 1 && currentGame % 2 == 1) {
			std::cout << "Jucatorul a castigat!\n";
			++playerScore;
		}
		else if (isGameOver(board, 'O') == 1 && currentGame % 2 == 1) {
			std::cout << "Algoritmul a castigat!\n";
			++algorithmScore;
		}
		else if (isGameOver(board, 'X') == 1 && currentGame % 2 == 0) {
			std::cout << "Algoritmul a castigat!\n";
			++algorithmScore;
		}
		else if (isGameOver(board, 'O') == 1 && currentGame % 2 == 0) {
			std::cout << "Jucatorul a castigat!\n";
			++playerScore;
		}

		initializeBoard(board);

		++currentGame;

	} while (askToContinue(playerScore, algorithmScore));

	return 0;
}
