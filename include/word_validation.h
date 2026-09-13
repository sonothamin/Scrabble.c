#ifndef WORD_VALIDATION_H
#define WORD_VALIDATION_H

#include <stdbool.h>
#include "board.h"
#include "dictionary.h"

typedef enum
{
    SCORE_BONUS_NONE = 0,
    SCORE_BONUS_2L,
    SCORE_BONUS_3L,
    SCORE_BONUS_2W,
    SCORE_BONUS_3W
} ScoreBonusKind;

bool Is_Word_In_Dictionary(const char *word, const Dictionary *dictionary);

int Calculate_Word_Score(const Tile *tiles, const int *xs, const int *ys, int wordLength,
                         const Tile previous_Grid[BOARD_SIDE][BOARD_SIDE],
                         const LuxuryType cells[BOARD_SIDE][BOARD_SIDE],
                         bool applyLuxuries, ScoreBonusKind *inoutBestBonus);

int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE],
                           Tile previous_Grid[BOARD_SIDE][BOARD_SIDE],
                           const LuxuryType cells[BOARD_SIDE][BOARD_SIDE],
                           bool applyLuxuries,
                           const Dictionary *dictionary,
                           ScoreBonusKind *outBestBonus);

#endif
