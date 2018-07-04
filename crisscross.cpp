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
#include "htmlformat.h"

struct option long_options[] = {
	{"print-solution", no_argument, nullptr, 'p'},
	{"formatter", required_argument, NULL, 'f'},
  {"rows", required_argument, NULL, 'r'},
  {"cols", required_argument, NULL, 'c'},
  {"max-words", required_argument, NULL, 'm'},
  {"aux-letters", required_argument, NULL, 'a'},
	{nullptr, 0, nullptr, 0}
};

void show_help(int argc, char **argv) {
  fprintf(stderr, "Usage: %s [-p|--print-solution] [-f|--format ascii|html] [-c|--cols num] [-r|--rows num] [-m|--max-words num] [-a|--auxilary_letters num]\n", argv[0]);
}

int to_int(const char* str, int* val) {
	char* end;
	int result = strtol(str, &end, 10);
	if(str + strlen(str) != end) {
		return 0;
	}

	*val = result;
	return 1;
}

int main(int argc, char **argv) {
	bool print_solution = false;
	void (*formatter)(const Board&, const std::vector<Word>&, bool) = ascii_format;
  int rows = 40, cols = 40;
  int max_words = 20;
  int auxilary_letters = 0;

	char ch;
	while ((ch = getopt_long(argc, argv, "pf:r:c:m:a:", long_options, nullptr)) != -1) {
		switch(ch) {
			case 'p':
				print_solution = true;
				break;
			case 'f':
				if(strcmp("html", optarg) == 0) {
          formatter = html_format;
				} else if(strcmp("ascii", optarg) == 0) {
          formatter = ascii_format;
        } else {
          fprintf(stderr, "Unknown formatter '%s'\n", optarg);
          exit(1);
        }
        break;
      case 'r':
        if(!to_int(optarg, &rows) || rows <= 0) {
          fprintf(stderr, "rows must be positive integer\n");
          exit(1);
        }
        break;
      case 'c':
        if(!to_int(optarg, &cols) || cols <= 0) {
          fprintf(stderr, "cols must be positive integer\n");
          exit(1);
        }
        break;
      case 'm':
        if(!to_int(optarg, &max_words) || max_words <= 0) {
          fprintf(stderr, "max-words must be positive integer\n");
          exit(1);
        }
        break;
      case 'a':
        if(!to_int(optarg, &auxilary_letters) || auxilary_letters <= 0) {
          fprintf(stderr, "auxilary_letters must be positive integer\n");
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

	Board board(cols, rows);
	board.putWord(1, 1, 1, words.at(1));

	std::vector<Word> used;
	while(used.size() < max_words) {
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
  board.fillAuxilaries(auxilary_letters);

	formatter(board, used, print_solution);
}
