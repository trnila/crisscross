#pragma once
#include <vector>
#include "board.h"
#include "word.h"

void html_format(const Board &board, const std::vector<Word> &used, bool print_solution);
