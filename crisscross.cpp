#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <stdlib.h>
#include <getopt.h>
#include <cstring>
#include "word.h"
#include "board.h"
#include "asciiformat.h"

void printLegend(std::vector<Word> &used);

struct option long_options[] = {
	{"print-solution", no_argument, nullptr, 'p'},
	{"formatter", required_argument, NULL, 'f'},
	{nullptr, 0, nullptr, 0}
};

enum class Formatter {
	ASCII,
	HTML
};

void show_help(int argc, char **argv) {
  fprintf(stderr, "Usage: %s [-p|--print-solution] [-f|--format ascii|html]\n", argv[0]);
}

int main(int argc, char **argv) {
	bool print_solution = false;
	Formatter choosed_formatter = Formatter::ASCII;

	char ch;
	while ((ch = getopt_long(argc, argv, "pf:", long_options, NULL)) != -1) {
		switch(ch) {
			case 'p':
				print_solution = true;
				break;
			case 'f':
				if(strcmp("html", optarg) == 0) {
          choosed_formatter = Formatter::HTML;
				} else if(strcmp("ascii", optarg) == 0) {
          choosed_formatter = Formatter::ASCII;
        } else {
          fprintf(stderr, "Unknown formatter '%s'\n", optarg);
          exit(1);
        }
        break;
      default:
        show_help(argc, argv);
        exit(1);
		}
	}


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

	Board board(35, 20);
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

	ascii_format(board, print_solution);

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
