#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "game.h"
#include "raylib.h"

typedef struct GameLayout
{
    Rectangle boardOuterBounds;
    Rectangle boardBounds;
    Rectangle detailsRect;
    Rectangle scoreRect;
    Rectangle historyOuterRect;
    Rectangle turnRect;
    Rectangle activeRackRect;
    Rectangle actionBtnRect;

    float tileSize;
    float tileSpacing;
    float rightSideX;
    float rightSideWidth;
    float layoutGap;
    float padding;
} GameLayout;

GameLayout CalculateGameLayout(const GameState *match);
void DrawGamePlayView(AppState *state, GameState *match);

#endif