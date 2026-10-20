#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "board.h"
#include "bag.h"
#include "player.h"
#include "drag_drop.h"
#include "shuffle.h"
#include "dictionary.h"
#include "gui_window_file_dialog.h"
#include "save_and_exit.h"
#include "wild_tile.h"
#include "word_validation.h"

#define MAX_PLAYER_TILES 7
#define MAX_TILE_BAG_SIZE 100
#define MAX_HISTORY_LOGS 20
#define MAX_WORD_LEN 15
#define MAX_HISTORY_ENTRIES 100

typedef struct
{
    char letter;
    int value;
    LuxuryType luxury;
} HistoryTileInfo;

typedef struct
{
    int playerIdx;
    HistoryTileInfo tiles[BOARD_SIDE];
    int tileCount;
    char word[32];
    int scoreEarned;
    ScoreBonusKind bonus;
} TurnHistoryEntry;

typedef struct
{
    float p1TimeRemaining; // Seconds remaining for Player 1
    float p2TimeRemaining; // Seconds remaining for Player 2
    bool isRunning;
} ChessTimerState;

typedef struct AppState AppState;

typedef enum
{
    GAME_MODE_LOCAL_1V1 = 0,
    GAME_MODE_ONLINE_MATCH
} GameMode;

typedef struct GameState
{
    GameMode mode;
    char dictionaryPath[256];
    bool specialTilesEnabled;
    GameBoard board;
    GameBoard previousBoard;
    Dictionary dictionary;
    Player players[2];
    int activePlayerIdx;
    unsigned int currentTurnNumber;
    TileBag tileBag;
    int tileBagCount;
    int consecutivePassCount;
    bool isMatchOver;
    bool isTie;
    int winningPlayerIdx;
    DragNDropState dragState;
    ShuffleState shuffleState;
    SaveAndExitOverlayState saveExitState;
    WildTileOverlayState wildTileState;
    TurnHistoryEntry historyEntries[MAX_HISTORY_ENTRIES];
    int historyCount;
    float historyScrollOffset;
    ChessTimerState timerState;
} GameState;

void GameInit(GameState *match, const char *dictionaryPath, const char *boardLayoutPath, const char *tileMapPath);
void GameUpdate(AppState *state);
void GameDraw(AppState *state);
void GameCleanUp(GameState *match);

#endif // GAME_H