#ifndef WORD_VALIDATION_H
#define WORD_VALIDATION_H

#include "board.h"
#include <stdbool.h>

typedef struct Dictionary
{
    char **words; // array of addresses of individual words in RAM
    int size;
    int capacity;
}Dictionary;

bool Load_Dictionary(Dictionary *dictionary, const char *dictionary_file);
void Free_Dictionary(Dictionary *dictionary);

bool Is_Word_In_Dictionary(const char *word, const Dictionary *dictionary);
int Calculate_Word_Score(const char *word);
int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE], Tile previous_Grid[BOARD_SIDE][BOARD_SIDE], const Dictionary *dictionary);

#endif