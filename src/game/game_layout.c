#include "game_layout.h"
#include <math.h>

GameLayout CalculateGameLayout(int screenWidth, int screenHeight, int baseFontSize)
{
    GameLayout layout;

    layout.padding = screenWidth * 0.03f;
    layout.layoutGap = screenWidth * 0.02f;
    layout.rightSideX = (screenWidth * 0.45f) + layout.padding + layout.layoutGap;
    layout.rightSideWidth = screenWidth - layout.rightSideX - layout.padding;
    layout.boardColWidth = screenWidth * 0.45f;
    layout.hkBarHeight = baseFontSize * 2.2f;
    layout.hkBarMargin = layout.padding * 0.5f;

    float boardGroupH = screenHeight - (layout.padding * 2.0f) - layout.hkBarMargin - layout.hkBarHeight - layout.hkBarMargin;
    layout.boardBoundaries = (Rectangle){layout.padding, layout.padding, layout.boardColWidth, boardGroupH};

    float groupBoxHeaderHeight = 25.0f;
    float gridInnerPad = 15.0f;
    float usableWidth = layout.boardBoundaries.width - (gridInnerPad * 2.0f);
    float usableHeight = layout.boardBoundaries.height - groupBoxHeaderHeight - (gridInnerPad * 2.0f);
    float boardVisualSize = fminf(usableWidth, usableHeight);
    float boardX = layout.boardBoundaries.x + (layout.boardBoundaries.width - boardVisualSize) / 2.0f;
    float boardY = layout.boardBoundaries.y + groupBoxHeaderHeight + (usableHeight - boardVisualSize) / 2.0f;

    layout.boardBounds = (Rectangle){boardX, boardY, boardVisualSize, boardVisualSize};
    layout.cellSize = boardVisualSize / 15.0f; // Assumes BOARD_SIDE = 15

    layout.topPanelsY = layout.padding;
    layout.topPanelsHeight = screenHeight * 0.18f;
    layout.halfRightWidth = (layout.rightSideWidth - layout.layoutGap) / 2.0f;

    layout.detailsRect = (Rectangle){layout.rightSideX, layout.topPanelsY, layout.halfRightWidth, layout.topPanelsHeight};
    layout.scoreRect = (Rectangle){layout.rightSideX + layout.halfRightWidth + layout.layoutGap, layout.topPanelsY, layout.halfRightWidth, layout.topPanelsHeight};

    layout.rackSectionY = layout.topPanelsY + layout.topPanelsHeight + layout.layoutGap;
    layout.rackPanelHeight = screenHeight * 0.10f;
    layout.rackRect = (Rectangle){layout.rightSideX, layout.rackSectionY, layout.rightSideWidth, layout.rackPanelHeight};
    layout.activeTileSize = layout.rackPanelHeight * 0.6f;
    layout.activeTileSpacing = 8.0f;

    layout.actionBarH = (float)(int)(baseFontSize * 2.0f);
    layout.actionBarY = layout.rackSectionY + layout.rackPanelHeight + (layout.layoutGap * 0.35f);
    layout.actionBarRect = (Rectangle){layout.rightSideX, layout.actionBarY, layout.rightSideWidth, layout.actionBarH};

    float actionBarTotalH = layout.actionBarH + (layout.layoutGap * 0.35f);
    layout.historySectionY = layout.rackSectionY + layout.rackPanelHeight + actionBarTotalH + (layout.layoutGap * 0.35f);
    float bottomRowHeight = screenHeight * 0.07f;
    layout.historyPanelHeight = screenHeight - layout.historySectionY - bottomRowHeight - layout.padding - layout.layoutGap;
    layout.historyOuterRect = (Rectangle){layout.rightSideX, layout.historySectionY, layout.rightSideWidth, layout.historyPanelHeight};

    layout.footerY = screenHeight - bottomRowHeight - layout.padding;
    layout.elementH = 45.0f;
    layout.elementY = layout.footerY + (bottomRowHeight - layout.elementH) / 2.0f;

    layout.actionBtnWidth = layout.rightSideWidth * 0.28f;
    layout.submitBtnWidth = layout.rightSideWidth * 0.30f;
    layout.turnIndicatorWidth = layout.rightSideWidth - layout.actionBtnWidth - layout.submitBtnWidth - layout.layoutGap;

    layout.turnRect = (Rectangle){layout.rightSideX, layout.elementY, layout.turnIndicatorWidth, layout.elementH};
    layout.submitBtnRect = (Rectangle){layout.rightSideX + layout.rightSideWidth - layout.actionBtnWidth - layout.submitBtnWidth - (layout.layoutGap * 0.5f), layout.elementY, layout.submitBtnWidth, layout.elementH};
    layout.actionBtnRect = (Rectangle){layout.rightSideX + layout.rightSideWidth - layout.actionBtnWidth, layout.elementY, layout.actionBtnWidth, layout.elementH};

    return layout;
}