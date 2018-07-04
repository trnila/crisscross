#include <cstdio>
#include <algorithm>
#include "board.h"

static void print_legend(const std::vector<Word> &used_words) {
  std::vector<Word> used(used_words); 

	sort(used.begin(), used.end(), [](Word &a, Word &b) {
		return a.size() < b.size();
	});
	int current = 0;
	for(auto word: used) {
		if(current != word.size()) {
			if(current != 0) {
				printf("<br>");
			}
			printf("%d - ", word.size());
			current = word.size();
		} else {
			printf(", ");
		}
		printWord(word);
	}
}

void html_format(const Board &board, const std::vector<Word> &used, bool print_solution) {
  std::cout << "<html><head><meta charset=utf-8>";
  FILE *f = fopen("style.css", "r");
  if(f) {
    char buffer[1];
    size_t r;
    printf("<style>");
    while((r = fread(buffer, sizeof(buffer), 1, f)) > 0) {
      fwrite(buffer, r, 1, stdout);
    }
    printf("</style>");
  }

  std::cout << "</head><body><table>";
  for(int r = 0; r < board.getHeight(); r++) {
    std::cout << "<tr>";
    for(int c = 0; c < board.getWidth(); c++) {
      std::cout << "<td"
                  << (board.isEmpty(c, r) ? "" : " class='cell'")
                  << ">"
                  << (board.canHint(c, r) ? utfchar2str(board.get(c, r)).c_str() : " ")
                  << "</td>";
    }
    std::cout << "</tr>";
  }
  std::cout << "</table>";

  print_legend(used);
  std::cout << "</body></html>";
}
