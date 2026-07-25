#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <stdbool.h>
#include "raylib.h"
#include "player.h"
#include "bag.h"

typedef struct GameState GameState;
typedef struct AppState  AppState;

typedef struct ShuffleState
{
    bool isActive;
    bool selected[RACK_SIZE];
    int  selectedCount;
} ShuffleState;

// Open / close helpers
void ShuffleOpen(ShuffleState *shuffle);
void ShuffleCancel(ShuffleState *shuffle);
bool ShuffleCommit(ShuffleState *shuffle, Player *player, TileBag *bag, int *bagCountOut);

// Independent modal: pass screen dims and base font size only.
// ShuffleUpdate handles click input inside the modal.
// ShuffleDraw renders the full floating modal; returns:
//   0  = no action
//   1  = Confirm pressed
//  -1  = Cancel pressed
void ShuffleUpdate(ShuffleState *shuffle, const Player *player, int screenW, int screenH, int baseFontSize);
int  ShuffleDraw (ShuffleState *shuffle, const Player *player, int screenW, int screenH, int baseFontSize);

#endif // SHUFFLE_H
