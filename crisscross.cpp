#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <stdlib.h>
#include "word.h"
#include "board.h"
#include "asciiformat.h"

void printLegend(std::vector<Word> &used);

int main() {
	std::ifstream in("words");
	if(!in.is_open()) {
		printf("Could not open dictionary!");
		return 1;
	}

	std::vector<Word> words;
	std::string w;
	while(std::getline(in, w)) {
		words.push_back(strToWord(w));
	}

	srand(time(NULL));
	int seed = rand();
	srand(seed);

	printf("seed: %d\n", seed);
	//srand(901720306);

	Board board(40, 20);
	board.putWord(1, 1, 1, words.at(1));

	std::vector<Word> used;
	while(used.size() < 50) {
		Word word = words.at(rand() % words.size());

		if(std::find(used.begin(), used.end(), word) != used.end()) {
			continue;
		}

		int x = rand() % board.getWidth();
		int y = rand() % board.getHeight();
		int right = rand() % 2;

		if(board.getScore(x, y, right, word) > 0) {
			board.putWord(x, y, right, word);
			used.push_back(word);
		}
	}

	format(board);

	printLegend(used);
}

void printLegend(std::vector<Word> &used) {
	sort(used.begin(), used.end(), [](Word &a, Word &b) {
		return a.size() < b.size();
	});
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
