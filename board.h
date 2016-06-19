#pragma once

#include "word.h"

class Board {
public:
	Board(int width, int height);
	~Board();

	Char get(int x, int y);
	bool isEmpty(int x, int y);

	bool putWord(int x, int y, bool right, Word word);
	int getScore(int x, int y, bool right, Word word);

	int getWidth() const;
	int getHeight() const;

	void print();

private:
	int width;
	int height;
	Char *board;

};


