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

void ShuffleOpen(ShuffleState *shuffle);
void ShuffleCancel(ShuffleState *shuffle);
bool ShuffleCommit(ShuffleState *shuffle, Player *player, TileBag *bag, int *bagCountOut);

//   0  = no action
//   1  = Confirm pressed
//  -1  = Cancel pressed

void ShuffleUpdate(ShuffleState *shuffle, const Player *player, int screenW, int screenH, int baseFontSize);
int  ShuffleDraw (ShuffleState *shuffle, const Player *player, int screenW, int screenH, int baseFontSize);

#endif // SHUFFLE_H
