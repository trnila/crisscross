#include <iostream>
#include "board.h"
#include "asciiformat.h"


int main() {
	Board board = Board::createFrom(std::cin);

	//board.print();
	format(board);
	return 0;
}