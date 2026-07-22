#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "board.h"
#include "bag.h"
#include "player.h"
#include "drag_drop.h"

#define MAX_PLAYER_TILES 7
#define MAX_TILE_BAG_SIZE 100
#define MAX_HISTORY_LOGS 20
#define MAX_WORD_LEN 15

typedef struct AppState AppState;

typedef enum
{
    GAME_MODE_LOCAL_1V1 = 0,
    GAME_MODE_AI_VS_PLAYER,
    GAME_MODE_ONLINE_MATCH
} GameMode;

typedef struct GameState
{
    GameMode mode;
    char dictionaryPath[256];
    bool specialTilesEnabled;
    GameBoard board;
    Player players[2];
    int activePlayerIdx;
    unsigned int currentTurnNumber;
    TileBag tileBag;
    int tileBagCount;
    bool isMatchOver;
    int winningPlayerIdx;
    DragNDropState dragState;
}GameState;

void GameInit(GameState *state);
void GameUpdate(AppState *state);
void GameDraw(AppState *state);

#endif // GAME_H