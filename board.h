#pragma once

#include "word.h"

class Board {
public:
	Board(int width, int height);
	~Board();

	static Board createFrom(std::istream &in);

	Char get(int x, int y) const;
	bool isEmpty(int x, int y) const;
  bool canHint(int x, int y) const;

	bool putWord(int x, int y, bool right, Word word);
	int getScore(int x, int y, bool right, Word word);

	int getWidth() const;
	int getHeight() const;

	void print();
	void save(std::ostream &out);
	void load(std::istream &in);

  void fillAuxilaries(int count);

private:
	int width;
	int height;

	Char *board;
  bool *auxilary;

	void set(int x, int y, Char c);
  int index(int x, int y) const;

};


