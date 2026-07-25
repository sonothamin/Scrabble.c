#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <stdbool.h>
#include "raylib.h"
#include "player.h"
#include "bag.h"

// Forward declaration
typedef struct GameState GameState;
typedef struct AppState  AppState;

// Per-tile selection state for the shuffle overlay
typedef struct ShuffleState
{
    bool isActive;                  // Is the shuffle overlay currently shown?
    bool selected[RACK_SIZE];       // Which rack tiles are selected for exchange
    int  selectedCount;             // Number of currently selected tiles
} ShuffleState;

// Open the shuffle overlay (called when the Shuffle button is pressed)
void ShuffleOpen(ShuffleState *shuffle);

// Close without committing (Cancel)
void ShuffleCancel(ShuffleState *shuffle);

// Commit: put selected tiles back in the bag, reshuffle bag, draw replacements
// Returns false if the bag has fewer tiles than selected tiles (not enough to swap)
bool ShuffleCommit(ShuffleState *shuffle, Player *player, TileBag *bag, int *bagCountOut);

// Handle click input for the overlay (tile selection)
// rackRect   – the rack group-box rectangle used to position tiles
// tileSize   – size of each tile square
// tileSpacing – gap between tiles
void ShuffleUpdate(ShuffleState *shuffle, const Player *player,
                   Rectangle rackRect, float tileSize, float tileSpacing);

// Draw the full-screen dimmed overlay with tile pickers and confirm/cancel buttons
// confirmBtnRect / cancelBtnRect – the positions of the two action buttons
// baseFontSize  – used for label sizing
// Returns: 1 = confirmed, -1 = cancelled, 0 = still open
int ShuffleDraw(ShuffleState *shuffle, const Player *player,
                Rectangle rackRect, float tileSize, float tileSpacing,
                Rectangle confirmBtnRect, Rectangle cancelBtnRect,
                int baseFontSize);

#endif // SHUFFLE_H
