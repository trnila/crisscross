#include <iostream>
#include "board.h"
#include "asciiformat.h"


int main() {
	Board board = Board::createFrom(std::cin);

	//board.print();
	//ascii_format(board, true);
	return 0;
}
