#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

#include "raylib.h"

typedef struct
{
    float padding;
    float layoutGap;
    float rightSideX;
    float rightSideWidth;
    float boardColWidth;
    float hkBarHeight;
    float hkBarMargin;
    Rectangle boardBoundaries;
    Rectangle boardBounds;
    float cellSize;
    float topPanelsY;
    float topPanelsHeight;
    float halfRightWidth;
    Rectangle detailsRect;
    Rectangle scoreRect;
    float rackSectionY;
    float rackPanelHeight;
    Rectangle rackRect;
    float activeTileSize;
    float activeTileSpacing;
    float actionBarH;
    float actionBarY;
    Rectangle actionBarRect;
    float historySectionY;
    float historyPanelHeight;
    Rectangle historyOuterRect;
    float footerY;
    float elementH;
    float elementY;
    float actionBtnWidth;
    float submitBtnWidth;
    float turnIndicatorWidth;
    Rectangle turnRect;
    Rectangle submitBtnRect;
    Rectangle actionBtnRect;
} GameLayout;

GameLayout CalculateGameLayout(int screenWidth, int screenHeight, int baseFontSize);

#endif