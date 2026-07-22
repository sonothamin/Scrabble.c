/*
 * game_input.c
 *
 * Handles all player input processing for the game screen.
 * Recalculates layout bounds and dispatches to HandleDragNDropInput().
 * Called exclusively from gameplay_core.c → GameUpdate().
 */

#include <math.h>
#include "game.h"
#include "app_state.h"
#include "raylib.h"
#include "drag_drop.h"

/* ------------------------------------------------------------------ */
/*  GameHandleInput                                                     */
/* ------------------------------------------------------------------ */
void GameHandleInput(AppState *state)
{
    GameState *match = state->gamestate;

    /* --- RECALCULATE BOUNDS FOR DRAG & DROP INPUT --- */
    const int screenWidth  = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const float padding       = screenWidth * 0.03f;
    const float layoutGap     = screenWidth * 0.02f;
    const float rightSideX    = (screenWidth * 0.45f) + padding + layoutGap;
    const float rightSideWidth = screenWidth - rightSideX - padding;

    /* Board Bounds */
    Rectangle boardBoundaries = {padding, padding, screenWidth * 0.45f, screenHeight - (padding * 2.0f)};
    float groupBoxHeaderHeight = 25.0f;
    float gridInnerPad         = 15.0f;
    float usableWidth          = boardBoundaries.width  - (gridInnerPad * 2.0f);
    float usableHeight         = boardBoundaries.height - groupBoxHeaderHeight - (gridInnerPad * 2.0f);
    float boardVisualSize      = fminf(usableWidth, usableHeight);
    float boardX = boardBoundaries.x + (boardBoundaries.width  - boardVisualSize) / 2.0f;
    float boardY = boardBoundaries.y + groupBoxHeaderHeight + (usableHeight - boardVisualSize) / 2.0f;
    Rectangle boardBounds = {boardX, boardY, boardVisualSize, boardVisualSize};

    /* Active Rack Bounds */
    float topPanelsY      = padding;
    float topPanelsHeight = screenHeight * 0.18f;
    float rackSectionY    = topPanelsY + topPanelsHeight + layoutGap;
    float rackPanelHeight = screenHeight * 0.10f;

    int p = match->activePlayerIdx;
    Rectangle activeRackRect = {rightSideX,
                                 rackSectionY + (p * (rackPanelHeight + (layoutGap * 0.5f))),
                                 rightSideWidth,
                                 rackPanelHeight};
    float tileSize    = rackPanelHeight * 0.6f;
    float tileSpacing = 8.0f;

    /* Process drag and drop interactions */
    HandleDragNDropInput(match, boardBounds, activeRackRect, tileSize, tileSpacing);
}
