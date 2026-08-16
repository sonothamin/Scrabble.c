#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "game.h"

void Action_ReturnStagedTilesToRack(GameState *match);
void Action_PassTurn(GameState *match);
void Action_UndoTurn(GameState *match);
void Action_SubmitMove(GameState *match);
bool Action_HasStagedTiles(const GameState *match);
void Action_FinalizeGameOver(GameState *match);

#endif