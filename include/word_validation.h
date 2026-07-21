#ifndef WORD_VALIDATION_H
#define WORD_VALIDATION_H

#include <stdbool.h>
#include "board.h"
#include "dictionary.h"

bool Is_Word_In_Dictionary(const char *word, const Dictionary *dictionary);
int Calculate_Word_Score(const char *word, const Tile *placedTiles, int wordLength);
int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE], Tile previous_Grid[BOARD_SIDE][BOARD_SIDE], const Dictionary *dictionary);

#endif