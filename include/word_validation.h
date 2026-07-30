#ifndef WORD_VALIDATION_H
#define WORD_VALIDATION_H

#include <stdbool.h>
#include "board.h"
#include "dictionary.h"

/** Highest premium square used by newly placed tiles in a scored move (for reward SFX). */
typedef enum
{
    SCORE_BONUS_NONE = 0,
    SCORE_BONUS_2L,
    SCORE_BONUS_3L,
    SCORE_BONUS_2W,
    SCORE_BONUS_3W
} ScoreBonusKind;

bool Is_Word_In_Dictionary(const char *word, const Dictionary *dictionary);

/**
 * Score one formed word with Scrabble premium rules.
 * Letter/word multipliers apply only for tiles newly placed this turn.
 */
int Calculate_Word_Score(const Tile *tiles, const int *xs, const int *ys, int wordLength,
                         const Tile previous_Grid[BOARD_SIDE][BOARD_SIDE],
                         const LuxuryType cells[BOARD_SIDE][BOARD_SIDE],
                         bool applyLuxuries, ScoreBonusKind *inoutBestBonus);

/**
 * Validate the current placement and return total points earned (0 = invalid / no score).
 * If outBestBonus is non-NULL, writes the strongest premium used by new tiles.
 */
int Scan_And_Validate_Move(Tile current_Grid[BOARD_SIDE][BOARD_SIDE],
                           Tile previous_Grid[BOARD_SIDE][BOARD_SIDE],
                           const LuxuryType cells[BOARD_SIDE][BOARD_SIDE],
                           bool applyLuxuries,
                           const Dictionary *dictionary,
                           ScoreBonusKind *outBestBonus);

#endif
