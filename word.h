#pragma once

#include <vector>
#include <cstdint>
#include <string>

typedef uint32_t Char;
typedef std::vector<Char> Word;

void printChar(Char c);
void printWord(Word word);
Word strToWord(std::string w);
