#include <cstdio>
#include <algorithm>
#include "board.h"

static void spaces(int n) {
	for(int i = 0; i < n; i++) {
		printf(" ");
	}
}

static void print_legend(std::vector<Word> &used) {
	int current = 0;
	for(auto word: used) {
		if(current != word.size()) {
			if(current != 0) {
				printf("\n");
			}
			printf("%d - ", word.size());
			current = word.size();
		} else {
			printf(", ");
		}
		printWord(word);
	}
}

void ascii_format(const Board &board, const std::vector<Word> &used, bool print_solution) {
	for (int y = 0; y < board.getHeight() + 1; ++y) {
		for(int c = 0; c < 2; c++) {
			int lastX = 0;
			int fix = 0;
			for (int x = 0; x < board.getWidth(); ++x) {
				if(board.get(x, y) != 0) {
					if(c == 0) {
						if(board.isEmpty(x - 1, y)) {
							spaces((x - lastX)*2 - fix);
							if(board.isEmpty(x, y - 1)) {
								if(board.isEmpty(x - 1, y - 1)) {
									printf("┌");
								}
							} else {
								if(board.isEmpty(x - 1, y - 1)) {
									printf("├");
								} else {
									printf("┼");
								}
							}
						} else {
							if(board.isEmpty(x - 1, y - 1) && board.isEmpty(x, y - 1) && !board.isEmpty(x - 1, y)) {
								printf("┬");
							} else {
								printf("┼");
							}
						}

						printf("─");

						if(board.isEmpty(x + 1, y)) {
							if(board.isEmpty(x, y - 1)) {
								if(!board.isEmpty(x + 1, y - 1)) {
									printf("┼");
								} else {
									printf("┐");
								}
								fix = 1;
							} else {
								if(!board.isEmpty(x + 1, y - 1)) {
									printf("┼");
								} else {
									printf("┤");
									fix = 1;
								}
							}
							lastX = x + 1;
						}
					} else if(c == 1) {
						spaces((x - lastX)*2 - fix);
						printf("│");
            if(print_solution) {
  						printChar(board.get(x, y));
            } else {
  						printf(" ");
            }

						if(board.isEmpty(x + 1, y)) {
							printf("│");
							fix = 1;
						}
						lastX = x+1;
					}
				} else {
					if(c == 0 && y != 0) {
						if(!board.isEmpty(x, y - 1)) {
							spaces((x - lastX)*2 - fix);
							if(board.isEmpty(x - 1, y - 1) && board.isEmpty(x - 1, y)) {
								printf("└");
							}
							printf("─");

							if(board.isEmpty(x + 1, y - 1)) {
								if(board.isEmpty(x + 1, y)) {
									printf("┘");
									fix = 1;
								} else {
									printf("┼");
								}
							} else if(board.isEmpty(x + 1, y)) {
								printf("┴");
							}


							lastX = x+1;
						}
					}
				}
			}

			if(y < board.getHeight()) {
				printf("\n");
			}
		}
	}

}
