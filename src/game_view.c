#include "game_view.h"
#include "drag_drop.h"
#include "raylib.h"
#include "raygui.h"
#include "ui.h"
#include <stdio.h>
#include "game.h"
#include "error_service.h"
#include "app_state.h"

GameLayout CalculateGameLayout(const GameState *match)
{
    GameLayout layout = {0};
    if (!match)
        return layout;

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const float padding = screenWidth * 0.03f;
    const float layoutGap = screenWidth * 0.02f;
    const float rightSideX = (screenWidth * 0.45f) + padding + layoutGap;
    const float rightSideWidth = screenWidth - rightSideX - padding;

    // --- BOARD BOUNDS ---
    Rectangle boardBoundaries = {padding, padding, screenWidth * 0.45f, screenHeight - (padding * 2.0f)};
    float groupBoxHeaderHeight = 25.0f;
    float gridInnerPad = 15.0f;
    float usableWidth = boardBoundaries.width - (gridInnerPad * 2.0f);
    float usableHeight = boardBoundaries.height - groupBoxHeaderHeight - (gridInnerPad * 2.0f);
    float boardVisualSize = fminf(usableWidth, usableHeight);
    float boardX = boardBoundaries.x + (boardBoundaries.width - boardVisualSize) / 2.0f;
    float boardY = boardBoundaries.y + groupBoxHeaderHeight + (usableHeight - boardVisualSize) / 2.0f;

    layout.boardBounds = (Rectangle){boardX, boardY, boardVisualSize, boardVisualSize};

    // --- ACTIVE RACK BOUNDS ---
    float topPanelsY = padding;
    float topPanelsHeight = screenHeight * 0.18f;
    float rackSectionY = topPanelsY + topPanelsHeight + layoutGap;
    float rackPanelHeight = screenHeight * 0.10f;

    int activeP = match->activePlayerIdx;
    layout.activeRackRect = (Rectangle){rightSideX, rackSectionY + (activeP * (rackPanelHeight + (layoutGap * 0.5f))), rightSideWidth, rackPanelHeight};

    layout.tileSize = rackPanelHeight * 0.6f;
    layout.tileSpacing = 8.0f;

    float historySectionY = rackSectionY + (rackPanelHeight * 2.0f) + (layout.layoutGap * 1.5f);
    float bottomRowHeight = screenHeight * 0.07f;
    float historyPanelHeight = screenHeight - historySectionY - bottomRowHeight - layout.padding - layout.layoutGap;

    layout.historyOuterRect = (Rectangle){layout.rightSideX, historySectionY, layout.rightSideWidth, historyPanelHeight};

    // 5. Bottom Action Bar (Turn Indicator & Save Button)
    float footerY = screenHeight - bottomRowHeight - layout.padding;
    float elementH = 45.0f;
    float elementY = footerY + (bottomRowHeight - elementH) / 2.0f;

    float actionBtnWidth = layout.rightSideWidth * 0.35f;
    float turnIndicatorWidth = layout.rightSideWidth - actionBtnWidth - layout.layoutGap;

    layout.turnRect = (Rectangle){layout.rightSideX, elementY, turnIndicatorWidth, elementH};
    layout.actionBtnRect = (Rectangle){layout.rightSideX + layout.rightSideWidth - actionBtnWidth, elementY, actionBtnWidth, elementH};

    return layout;
}

void DrawGamePlayView(AppState *state, GameState *match)
{
    if (!match)
    {
        ReportCriticalError("Invalid Game State", "Null GameState pointer encountered while drawing game view");
        return;
    }

    ClearBackground((Color){24, 32, 38, 255});
    int baseFontSize = fmaxf(15, GetScreenHeight() / 42);
    ApplyScrabbleTheme(baseFontSize);

    // Obtain layout geometry
    GameLayout layout = CalculateGameLayout(match);

    GuiGroupBox(layout.boardOuterBounds, "Game Board");
    DrawRectangle(layout.boardBounds.x, layout.boardBounds.y, layout.boardBounds.width, layout.boardBounds.height, (Color){34, 44, 52, 255});
    float cellSize = layout.boardBounds.width / (float)match->board.sideSize;

    DrawTheCellTypes(&match->board, layout.boardBounds.x, layout.boardBounds.y, layout.boardBounds.width, cellSize);

    for (int i = 0; i <= match->board.sideSize; i++)
    {
        DrawLineV((Vector2){layout.boardBounds.x + (i * cellSize), layout.boardBounds.y}, (Vector2){layout.boardBounds.x + (i * cellSize), layout.boardBounds.y + layout.boardBounds.height}, (Color){54, 68, 82, 100});
        DrawLineV((Vector2){layout.boardBounds.x, layout.boardBounds.y + (i * cellSize)}, (Vector2){layout.boardBounds.x + layout.boardBounds.width, layout.boardBounds.y + (i * cellSize)}, (Color){54, 68, 82, 100});
    }

    GuiGroupBox(layout.detailsRect, "Match Status");

    float tableY = layout.detailsRect.y + 25.0f;
    float rowHeight = (layout.detailsRect.height - 35.0f) / 3.0f;
    const char *keys[3] = {"Mode:", "Special Tiles:", "Tiles Left:"};

    const char *modeStr = (match->mode == GAME_MODE_LOCAL_1V1) ? "Local 1v1" : "AI vs Player";
    const char *tilesLeftStr = TextFormat("%d Tiles", match->tileBagCount);
    const char *values[3] = {modeStr, match->specialTilesEnabled ? "Enabled" : "Disabled", tilesLeftStr};

    for (int i = 0; i < 3; i++)
    {
        float currentRowY = tableY + (i * rowHeight);
        if (i % 2 == 0)
        {
            DrawRectangleRec((Rectangle){layout.detailsRect.x + 6.0f, currentRowY, layout.detailsRect.width - 12.0f, rowHeight}, (Color){30, 38, 46, 180});
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
        GuiLabel((Rectangle){layout.detailsRect.x + 12.0f, currentRowY, layout.detailsRect.width * 0.45f, rowHeight}, keys[i]);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
        GuiLabel((Rectangle){layout.detailsRect.x + 12.0f + (layout.detailsRect.width * 0.45f), currentRowY, (layout.detailsRect.width * 0.55f) - 18.0f, rowHeight}, values[i]);
    }

    GuiGroupBox(layout.scoreRect, "Current Score");

    float scoreColW = (layout.scoreRect.width - 20.0f) / 2.0f;
    int massiveFontSize = (int)(baseFontSize * 2.5f);
    float scoreTextY = layout.scoreRect.y + 25.0f;
    Color neonLime = (Color){58, 223, 0, 255};
    Color separatorColor = (Color){54, 68, 82, 255};

    for (int p = 0; p < 2; p++)
    {
        const char *scoreStr = TextFormat("%03d", match->players[p].score);
        float textX = layout.scoreRect.x + 15.0f + (p * scoreColW) + (scoreColW - MeasureText(scoreStr, massiveFontSize)) / 2.0f;
        DrawText(scoreStr, textX, scoreTextY, massiveFontSize, neonLime);
    }

    float horizontalLineY = scoreTextY + massiveFontSize + 12.0f;
    DrawLineV((Vector2){layout.scoreRect.x + 12.0f, horizontalLineY}, (Vector2){layout.scoreRect.x + layout.scoreRect.width - 12.0f, horizontalLineY}, separatorColor);
    DrawLineV((Vector2){layout.scoreRect.x + 15.0f + scoreColW, layout.scoreRect.y + 20.0f}, (Vector2){layout.scoreRect.x + 15.0f + scoreColW, layout.scoreRect.y + layout.scoreRect.height - 12.0f}, separatorColor);

    GuiSetStyle(LABEL, TEXT_SIZE, (int)(baseFontSize * 0.75f));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 1);

    GuiLabel((Rectangle){layout.scoreRect.x + 15.0f, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 1");
    GuiLabel((Rectangle){layout.scoreRect.x + 15.0f + scoreColW, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 2");

    GuiSetStyle(LABEL, TEXT_SIZE, baseFontSize);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0);

    // Center Section: Tile Racks
    float rackSectionY = layout.detailsRect.y + layout.detailsRect.height + layout.layoutGap;
    ;
    float rackPanelHeight = GetScreenHeight() * 0.10f;

    for (int p = 0; p < 2; p++)
    {
        Rectangle rackRect = {layout.rightSideX, rackSectionY + (p * (rackPanelHeight + (layout.layoutGap * 0.5f))), layout.rightSideWidth, rackPanelHeight};
        GuiGroupBox(rackRect, TextFormat("Player %d Rack", p + 1));

        float tileY = rackRect.y + (rackPanelHeight - layout.tileSize) / 2.0f + 4.0f;

        for (int t = 0; t < match->players[p].rack_count; t++)
        {
            // Don't render the tile inside the rack if it is currently being dragged
            if ((p == match->activePlayerIdx) && (match->dragState.isDragging) && (match->dragState.draggedTileIdx == t))
                continue;
            Tile tile = match->players[p].rack[t];
            Rectangle tileBounds = {rackRect.x + 15.0f + (t * (layout.tileSize + layout.tileSpacing)), tileY, layout.tileSize, layout.tileSize};
            DrawRectangleRounded(tileBounds, 0.2f, 4, (Color){244, 228, 198, 255});
            DrawRectangleRoundedLines(tileBounds, 0.2f, 4, (Color){194, 169, 126, 255});

            char letterStr[2] = {tile.letter, '\0'};
            int rackTileFontSize = (int)(layout.tileSize * 0.55f);
            DrawText(letterStr, tileBounds.x + (layout.tileSize * 0.15f), tileBounds.y + (layout.tileSize - rackTileFontSize) / 2.0f, rackTileFontSize, (Color){38, 28, 16, 255});

            const char *scoreStr = TextFormat("%d", tile.value);
            int scoreFontSize = (int)(layout.tileSize * 0.22f);
            DrawText(scoreStr, tileBounds.x + layout.tileSize - MeasureText(scoreStr, scoreFontSize) - (layout.tileSize * 0.12f), tileBounds.y + layout.tileSize - scoreFontSize - (layout.tileSize * 0.10f), scoreFontSize, (Color){80, 65, 50, 255});
        }
    }

    GuiGroupBox(layout.historyOuterRect, "Points History");

    DrawRectangleRec(layout.turnRect, (Color){33, 43, 51, 255});
    DrawRectangleLinesEx(layout.turnRect, 1.0f, separatorColor);

    Color activeAlertColor = (match->activePlayerIdx == 0) ? (Color){0, 220, 255, 255} : (Color){255, 180, 0, 255};
    const char *activeTurnStr = (match->activePlayerIdx == 0) ? "PLAYER 1" : "PLAYER 2";

    int turnFontSize = (int)(baseFontSize * 1.6f);
    float indicatorTextY = layout.turnRect.y + (layout.turnRect.height - turnFontSize) / 2.0f;

    DrawRectangle(layout.turnRect.x + 2, layout.turnRect.y + 2, 8, layout.turnRect.height - 4, activeAlertColor);

    DrawText("TURN:", layout.turnRect.x + 22.0f, indicatorTextY, turnFontSize, (Color){150, 165, 175, 255});
    DrawText(activeTurnStr, layout.turnRect.x + 22.0f + MeasureText("TURN: ", turnFontSize), indicatorTextY, turnFontSize, activeAlertColor);

    if (GuiButton(layout.actionBtnRect, "Save & Exit Match"))
    {
        state->currentScreen = APP_SCREEN_MAIN_MENU;
    }

    DrawDragNDropOverlay(match, layout.activeRackRect, layout.tileSize, layout.tileSpacing);
}
