#include <ostream>
#include <sstream>
#include "word.h"

void printChar(Char c, std::ostream& out) {
	if(c == 0) {
		out << " ";
	} else if(c < 0x7F) {
		out << (char) c;
	} else {
		out << (char) (0xC0 | ((0x3C0&c)>>6))
		    << (char) (0x80 | (0x3F & c));
	}
}

std::string utfchar2str(Char c) {
  std::ostringstream out;
  printChar(c, out);
  return out.str();
}

void printWord(Word word) {
	for(int i = 0; i < word.size(); i++) {
		printChar(word.at(i));
	}
}

Word strToWord(std::string w) {
	Word chars;
	Char c = 0;
	int remaining = 0;
	for(unsigned char val: w) {
		if((val & 0x80) == 0) {
			if(val != '\r' && val != '\n') {
				remaining = 0;
				chars.push_back(val);
			}
		} else if((val & 0xC0) == 0x80) { // 10xx xxxx
			remaining--;
			c |= (val & 0x3F) << remaining * 6;

			if(remaining == 0) {
				chars.push_back(c);
			}
		} else if((val & 0xE0) == 0xC0) { // 110x xxxx
			remaining = 1;
			c = (val & 0x1F) << 6;
		} else {
			printf("err! %x\n", val);
		}
	}

	return chars;
}
