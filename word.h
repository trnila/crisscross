#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

typedef uint32_t Char;
typedef std::vector<Char> Word;

void printChar(Char c, std::ostream &out = std::cout);
void printWord(Word word);
std::string utfchar2str(Char c);

Word strToWord(std::string w);
