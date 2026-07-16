#ifndef WORD_VALIDATION_H
#define WORD_VALIDATION_H

#include "board.h"
#include <stdbool.h>

typedef struct PlayedWord
{
    char word[16];
    int score;
    bool isValid;
} PlayedWord;

bool is_Word_In_Dictionary(const char *word, const char *dictionary);
int Calculate_Word_Score(const char *word);
int Scan_And_Validate_Move(Tile currentGrid[BOARD_SIDE][BOARD_SIDE], Tile previousGrid[BOARD_SIDE][BOARD_SIDE], const char *dictionary);

#endif