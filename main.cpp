#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <assert.h>

typedef uint32_t Char;


void printChar(Char c) {
	if(c == 0) {
		printf(" ");
	} else if(c < 0x7F) {
		printf("%c", c, c);
	} else {
		printf(
			"%c%c",
			(0xC0 | ((0x3C0&c)>>6)),
			0x80 | (0x3F & c)
		);
	}
}

void printWord(std::vector<Char> word) {
	for(int i = 0; i < word.size(); i++) {
		printChar(word.at(i));
	}
}

const int N = 10;
Char board[N][N];

std::vector<std::vector<Char>> words;

bool isFree(int x, int y) {
	if(x < 0 || x >= N || y < 0 || y >= N) {
		return true;
	}

	return board[x][y] == 0;
}

int willFit(int x, int y, bool right, std::vector<Char> word) {
	if((right && x + word.size() >= N) || (!right && y + word.size() >= N)) {
		return -1;
	}

	if(right && (!isFree(x - 1, y) || !isFree(x + word.size() + 1, y))) {
		return -1;
	}

	if(!right && (!isFree(x, y - 1) || !isFree(x, y + word.size() + 1))) {
		return -1;
	}


	int score = 0;
	for(int i = 0; i < word.size(); i++) {
		if(board[x][y] != 0 && board[x][y] != word.at(i)) {
			return -1;
		}

		if(board[x][y] == 0) {
			if(right) {
				if(board[x][y-1] != 0 || board[x][y+1] != 0) {
					return -1;
				}
			} else {
				if(board[x-1][y] != 0 || board[x+1][y] != 0) {
					return -1;
				}
			}
		}


		if(board[x][y] != 0) {
			score++;
		}

		if(right) {
			x++;
		} else {
			y++;
		}
	}

	return score;
}

void printTable() {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printChar(board[j][i]);
		}
		printf("\n");
	}
}

void placeWord(int x, int y, bool right, std::vector<Char> word) {
	for(int i = 0; i < word.size(); i++) {
		assert(board[x][y] == 0 || board[x][y] == word.at(i));
		assert(x < N && y < N);

		board[x][y] = word.at(i);

		if(right) {
			x++;
		} else {
			y++;
		}
	}
}

int main() {
	std::ifstream in("words");

	std::string w;
	while(std::getline(in, w)) {
		std::vector<Char> chars;
		Char c = 0;
		int remaining = 0;
		for(unsigned char val: w) {
			if((val & 0x80) == 0) {
				if(val != '\r' && val != '\n') {
					remaining = 0;
					chars.push_back(val);
				}
			} else if((val & 0xC0) == 0x80) { // 10xx xxxx
				remaining--;
				c |= (val & 0x3F) << remaining * 6;

				if(remaining == 0) {
					chars.push_back(c);
				}
			} else if((val & 0xE0) == 0xC0) { // 110x xxxx
				remaining = 1;
				c = (val & 0x1F) << 6;
			} else {
				printf("err! %x\n", val);
			}
		}
		words.push_back(chars);
	}

	/*for(Char c: chars) {
		printChar(c);
	}

	std::cout << "\n";

	board[5][4]=chars[0];*/

	srand(6);
	//srand(time(0));
	int placed = 0;

	placeWord(1, 1, 1, words.at(1));

	std::vector<std::vector<Char>> used;

	while(placed < 5) {
		std::vector<Char> word = words.at(rand() % 100);

		if(std::find(used.begin(), used.end(), word) != used.end()) {
			continue;
		}


		int x = rand() % N;
		int y = rand() % N;
		int right = rand() % 2;

		if(willFit(x, y, right, word) > 0) {
			placeWord(x, y, right, word);
			//printWord(word);
			placed++;
			used.push_back(word);
		}
	}

	printTable();


	for (int y = 0; y < N; ++y) {
		for(int c = 0; c < 3; c++) {
			for (int x = 0; x < N; ++x) {
				if(board[x][y] != 0) {
					if(c == 0) {
						if(isFree(x - 1, y)) {
							if(isFree(x, y - 1)) {
								printf("┌");
							} else {
								printf("├");
							}
						} else {
							if(isFree(x, y - 1)) {
								printf("┬");
							} else {
								printf("┼");
							}
						}

						printf("─");

						if(isFree(x + 1, y)) {
							if(isFree(x, y - 1)) {
								printf("┐");
							} else {
								printf("┤");
							}
						} else {
							if(isFree(x, y - 1)) {
								printf("┬");
							} else {
								printf("┼");
							}
						}
					} else if(c == 1) {
						printf("│ │");
					} else if(c == 2) {
						if(isFree(x - 1, y)) {
							if(isFree(x, y + 1)) {
								printf("└");
							} else {
								printf("├");
							}
						} else {
							if(isFree(x, y + 1)) {
								printf("┴");
							} else {
								printf("┼");
							}
						}

						printf("─");

						if(isFree(x + 1, y)) {
							if(isFree(x, y + 1)) {
								printf("┘");
							} else {
								printf("┤");
							}
						} else {
							if(isFree(x, y + 1)) {
								printf("┴");
							} else {
								printf("┼");
							}
						}
					}

					//printChar(board[j][i]);
				} else {
					printf("   ");
				}
			}
			printf("\n");
		}
	}

	std::sort(used.begin(), used.end(), [](std::vector<Char> &a, std::vector<Char> &b) {
		return a.size() < b.size();
	});
	for(auto word: used) {
		printWord(word);
		printf("\n");
	}
}