
#include <assert.h>
#include <cstdio>
#include "board.h"

Board::Board(int width, int height) : width(width), height(height) {
	board = new Char[width * height];
}

Board::~Board() {
	delete[] board;
}

Char Board::get(int x, int y) {
	if(x < 0 || x >= width || y < 0 || y >= height) {
		return 0;
	}

	return board[x * width + y];
}

bool Board::isEmpty(int x, int y) {
	return get(x, y) == 0;
}

bool Board::putWord(int x, int y, bool right, Word word) {
	for(int i = 0; i < word.size(); i++) {
		assert(get(x, y) == 0 || get(x, y) == word.at(i));
		assert(x < width && y < height);

		board[x * width + y] = word.at(i);

		if(right) {
			x++;
		} else {
			y++;
		}
	}
}

int Board::getScore(int x, int y, bool right, Word word) {
	if((right && x + word.size() >= width) || (!right && y + word.size() >= height)) {
		return -1;
	}

	if(right && (!isEmpty(x - 1, y) || !isEmpty(x + word.size() + 1, y))) {
		return -1;
	}

	if(!right && (!isEmpty(x, y - 1) || !isEmpty(x, y + word.size() + 1))) {
		return -1;
	}


	int score = 0;
	for(int i = 0; i < word.size(); i++) {
		if(get(x, y) != 0 && get(x, y) != word.at(i)) {
			return -1;
		}

		if(get(x, y) == 0) {
			if(right) {
				if(get(x, y-1) != 0 || get(x, y+1) != 0) {
					return -1;
				}
			} else {
				if(get(x-1, y) != 0 || get(x+1, y) != 0) {
					return -1;
				}
			}
		}


		if(get(x, y) != 0) {
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

int Board::getWidth() const {
	return width;
}

int Board::getHeight() const {
	return height;
}

void Board::print() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			printChar(get(j, i));
		}
		printf("\n");
	}
}
