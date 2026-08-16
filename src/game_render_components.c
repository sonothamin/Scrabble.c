#include "game_render_components.h"
#include "game_actions.h"
#include "ui.h"
#include "raygui.h"
#include "sound.h"
#include "shuffle.h"
#include "settings.h"
#include <math.h>

void Draw_BoardView(const GameState *match, const GameLayout *layout)
{
    GuiGroupBox(layout->boardBoundaries, "Game Board");
    DrawRectangle(layout->boardBounds.x, layout->boardBounds.y, layout->boardBounds.width, layout->boardBounds.height, (Color){34, 44, 52, 255});

    DrawTheCellTypes(&match->board, layout->boardBounds.x, layout->boardBounds.y, layout->boardBounds.width, layout->cellSize);

    for (int i = 0; i <= match->board.sideSize; i++)
    {
        DrawLineV((Vector2){layout->boardBounds.x + (i * layout->cellSize), layout->boardBounds.y},
                  (Vector2){layout->boardBounds.x + (i * layout->cellSize), layout->boardBounds.y + layout->boardBounds.height}, (Color){54, 68, 82, 100});
        DrawLineV((Vector2){layout->boardBounds.x, layout->boardBounds.y + (i * layout->cellSize)},
                  (Vector2){layout->boardBounds.x + layout->boardBounds.width, layout->boardBounds.y + (i * layout->cellSize)}, (Color){54, 68, 82, 100});
    }

    for (int r = 0; r < match->board.sideSize; r++)
    {
        for (int c = 0; c < match->board.sideSize; c++)
        {
            Tile placedTile = match->board.grid[r][c];
            if (placedTile.letter != '\0')
            {
                Rectangle cellBounds = {layout->boardBounds.x + (c * layout->cellSize) + 1, layout->boardBounds.y + (r * layout->cellSize) + 1, layout->cellSize - 2, layout->cellSize - 2};
                DrawRectangleRounded(cellBounds, 0.15f, 3, (Color){244, 228, 198, 255});
                DrawRectangleRoundedLines(cellBounds, 0.15f, 3, (Color){194, 169, 126, 255});

                char letterStr[2] = {placedTile.letter, '\0'};
                int tileFontSize = (int)(layout->cellSize * 0.55f);
                int textW = MeasureText(letterStr, tileFontSize);
                DrawAppText(letterStr, cellBounds.x + (layout->cellSize - textW) / 2.0f, cellBounds.y + (layout->cellSize - tileFontSize) / 2.0f, tileFontSize, (Color){38, 28, 16, 255});

                const char *scoreStr = TextFormat("%d", placedTile.value);
                int scoreFontSize = (int)(layout->cellSize * 0.40f);
                int scoreW = MeasureText(scoreStr, scoreFontSize);
                DrawAppText(scoreStr, cellBounds.x + layout->cellSize - scoreW - (layout->cellSize * 0.25f), cellBounds.y + layout->cellSize - scoreFontSize - (layout->cellSize * 0.06f), scoreFontSize, (Color){80, 65, 50, 255});
            }
        }
    }
}

void Draw_HotkeyView(const AppState *state, const GameLayout *layout, int baseFontSize)
{
    float hkBarY = layout->padding + layout->boardBoundaries.height + layout->hkBarMargin;
    bool isMuted = (state->settingsState != NULL && !state->settingsState->bgmEnable);
    HotkeyEntry liveKeys[5] = {
        {"P", "Pause"}, {"S", "Pass"}, {"Z", "Undo"}, {"M", isMuted ? "Unmute" : "Mute"}, {"Q", "Quit"}};

    DrawHotkeyBar(liveKeys, 5, layout->padding, hkBarY + layout->padding * 0.25f, layout->boardColWidth, layout->hkBarHeight, baseFontSize);
}

void Draw_MatchStatusView(const GameState *match, const GameLayout *layout)
{
    GuiGroupBox(layout->detailsRect, "Match Status");
    float tableY = layout->detailsRect.y + 25.0f;
    float rowHeight = (layout->topPanelsHeight - 35.0f) / 3.0f;

    const char *modeStr = (match->mode == GAME_MODE_LOCAL_1V1) ? "Local 1v1" : "Network";
    const char *bagCountStr = TextFormat("%d Tiles", match->tileBagCount);
    const char *consecutivePassesStr = TextFormat("%d / 6", match->consecutivePassCount);

    const char *keys[3] = {"Mode:", "Tiles Remaining:", "Pass Count:"};
    const char *values[3] = {modeStr, bagCountStr, consecutivePassesStr};

    for (int i = 0; i < 3; i++)
    {
        float currentRowY = tableY + (i * rowHeight);
        if (i % 2 == 0)
        {
            DrawRectangleRec((Rectangle){layout->detailsRect.x + 6.0f, currentRowY, layout->halfRightWidth - 12.0f, rowHeight}, (Color){30, 38, 46, 180});
        }
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
        GuiLabel((Rectangle){layout->detailsRect.x + 12.0f, currentRowY, layout->halfRightWidth * 0.48f, rowHeight}, keys[i]);
        GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
        GuiLabel((Rectangle){layout->detailsRect.x + 12.0f + (layout->halfRightWidth * 0.48f), currentRowY, (layout->halfRightWidth * 0.52f) - 18.0f, rowHeight}, values[i]);
    }
}

void Draw_ScoreBoardView(const GameState *match, const GameLayout *layout, int baseFontSize)
{
    GuiGroupBox(layout->scoreRect, "Current Score");
    float scoreColW = (layout->halfRightWidth - 20.0f) / 2.0f;
    int massiveFontSize = (int)(baseFontSize * 2.5f);
    float scoreTextY = layout->scoreRect.y + 25.0f;
    Color neonLime = (Color){58, 223, 0, 255};
    Color separatorColor = (Color){54, 68, 82, 255};

    for (int p = 0; p < 2; p++)
    {
        const char *scoreStr = TextFormat("%03d", match->players[p].score);
        float textX = layout->scoreRect.x + 15.0f + (p * scoreColW) + (scoreColW - MeasureAppText(scoreStr, massiveFontSize)) / 2.0f;
        DrawAppText(scoreStr, textX, scoreTextY, massiveFontSize, neonLime);
    }

    float horizontalLineY = scoreTextY + massiveFontSize + 12.0f;
    DrawLineV((Vector2){layout->scoreRect.x + 12.0f, horizontalLineY}, (Vector2){layout->scoreRect.x + layout->scoreRect.width - 12.0f, horizontalLineY}, separatorColor);
    DrawLineV((Vector2){layout->scoreRect.x + 15.0f + scoreColW, layout->scoreRect.y + 20.0f}, (Vector2){layout->scoreRect.x + 15.0f + scoreColW, layout->scoreRect.y + layout->scoreRect.height - 12.0f}, separatorColor);

    GuiSetStyle(LABEL, TEXT_SIZE, (int)(baseFontSize * 0.75f));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0x8F8FA0FF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 1);

    GuiLabel((Rectangle){layout->scoreRect.x + 15.0f, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 1");
    GuiLabel((Rectangle){layout->scoreRect.x + 15.0f + scoreColW, horizontalLineY + 8.0f, scoreColW, 20}, "PLAYER 2");

    GuiSetStyle(LABEL, TEXT_SIZE, baseFontSize);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xAACF9BFF);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, 0);
}

void Draw_RackView(const GameState *match, const GameLayout *layout, int baseFontSize)
{
    int p = match->activePlayerIdx;
    GuiGroupBox(layout->rackRect, TextFormat("Player %d Rack (Active Turn)", p + 1));
    float tileY = layout->rackRect.y + (layout->rackPanelHeight - layout->activeTileSize) / 2.0f + 4.0f;

    for (int t = 0; t < match->players[p].rack_count; t++)
    {
        if ((match->dragState.isDragging) && (match->dragState.draggedTileIdx == t))
            continue;

        Tile tile = match->players[p].rack[t];
        Rectangle tileBounds = {layout->rackRect.x + 15.0f + (t * (layout->activeTileSize + layout->activeTileSpacing)),
                                tileY, layout->activeTileSize, layout->activeTileSize};
        DrawRectangleRounded(tileBounds, 0.2f, 4, (Color){244, 228, 198, 255});
        DrawRectangleRoundedLines(tileBounds, 0.2f, 4, (Color){194, 169, 126, 255});

        char letterStr[2] = {tile.letter, '\0'};
        int rackTileFontSize = (int)(layout->activeTileSize * 0.55f);
        DrawAppText(letterStr, tileBounds.x + (layout->activeTileSize * 0.15f),
                    tileBounds.y + (layout->activeTileSize - rackTileFontSize) / 2.0f,
                    rackTileFontSize, (Color){38, 28, 16, 255});

        const char *scoreStr = TextFormat("%d", tile.value);
        int scoreFontSize = (int)(layout->cellSize * 0.40f);
        DrawAppText(scoreStr,
                    tileBounds.x + layout->activeTileSize - MeasureAppText(scoreStr, scoreFontSize) - (layout->activeTileSize * 0.25f),
                    tileBounds.y + layout->activeTileSize - scoreFontSize - (layout->activeTileSize * 0.10f),
                    scoreFontSize, (Color){80, 65, 50, 255});
    }

    float bagTextX = layout->rackRect.x + 15.0f + (match->players[p].rack_count * (layout->activeTileSize + layout->activeTileSpacing)) + 10.0f;
    const char *mutedBagText = TextFormat("+%d Tiles", match->tileBagCount);
    int mutedFontSize = (int)(baseFontSize * 0.9f);
    int textWidth = MeasureAppText(mutedBagText, mutedFontSize);

    if (bagTextX + textWidth < layout->rackRect.x + layout->rightSideWidth - 12.0f)
    {
        float mutedTextY = layout->rackRect.y + (layout->rackPanelHeight - mutedFontSize) / 2.0f + 2.0f;
        DrawAppText(mutedBagText, bagTextX, mutedTextY, mutedFontSize, (Color){140, 155, 165, 200});
    }
}

void Draw_ActionBarView(GameState *match, const GameLayout *layout, int baseFontSize)
{
    DrawRectangleRec(layout->actionBarRect, (Color){22, 30, 36, 220});
    DrawRectangleLinesEx(layout->actionBarRect, 1.0f, (Color){54, 68, 82, 180});

    float abPad = 6.0f;
    float abBtnH = layout->actionBarH - abPad * 2.0f;
    float abBtnW = layout->rightSideWidth * 0.23f;
    float abGap = 6.0f;

    int badgeFontSize = (int)(baseFontSize * 0.78f);
    const char *passCountStr = TextFormat("Passes: %d/6", match->consecutivePassCount);
    int passCountW = MeasureAppText(passCountStr, badgeFontSize);
    Color passCountColor = (match->consecutivePassCount > 0) ? (Color){255, 80, 70, 255} : (Color){90, 105, 120, 180};
    DrawAppText(passCountStr,
                layout->actionBarRect.x + layout->actionBarRect.width - passCountW - abPad - 2.0f,
                layout->actionBarRect.y + (layout->actionBarH - badgeFontSize) / 2.0f,
                badgeFontSize, passCountColor);

    Rectangle passBtn = {layout->actionBarRect.x + abPad, layout->actionBarRect.y + abPad, abBtnW, abBtnH};
    Rectangle shuffleBtn = {passBtn.x + abBtnW + abGap, layout->actionBarRect.y + abPad, abBtnW, abBtnH};
    Rectangle undoBtn = {shuffleBtn.x + abBtnW + abGap, layout->actionBarRect.y + abPad, abBtnW, abBtnH};

    if (GuiButton(passBtn, "Pass") && !match->shuffleState.isActive)
    {
        Action_PassTurn(match);
    }

    if (GuiButton(shuffleBtn, "Shuffle") && !match->shuffleState.isActive)
    {
        ShuffleOpen(&match->shuffleState);
        PlaySoundEffect(SFX_BUTTON);
    }

    bool hasStagedTiles = Action_HasStagedTiles(match);
    if (!hasStagedTiles)
        GuiSetState(STATE_DISABLED);
    if (GuiButton(undoBtn, "Undo Turn") && hasStagedTiles && !match->shuffleState.isActive)
    {
        Action_UndoTurn(match);
    }
    if (!hasStagedTiles)
        GuiSetState(STATE_NORMAL);
}

void Draw_HistoryView(const GameLayout *layout, int baseFontSize)
{
    GuiGroupBox(layout->historyOuterRect, "Points History");
    float historySplitWidth = (layout->rightSideWidth - (layout->layoutGap * 1.5f)) / 2.0f;
    float historyInnerY = layout->historySectionY + 25.0f;

    for (int hp = 0; hp < 2; hp++)
    {
        Rectangle subHistoryRect = {layout->historyOuterRect.x + 12.0f + (hp * (historySplitWidth + layout->layoutGap)),
                                    historyInnerY, historySplitWidth, layout->historyPanelHeight - 40.0f};

        DrawRectangleRec(subHistoryRect, (Color){30, 38, 46, 255});
        DrawRectangleLinesEx(subHistoryRect, 1.0f, (Color){54, 68, 82, 255});
        DrawRectangle(subHistoryRect.x, subHistoryRect.y, subHistoryRect.width, 28, (Color){45, 55, 66, 255});
        DrawAppText(TextFormat("Player %d History", hp + 1), subHistoryRect.x + 12, subHistoryRect.y + 7, baseFontSize - 2, (Color){196, 181, 137, 255});
    }
}

void Draw_FooterView(GameState *match, const GameLayout *layout, int baseFontSize)
{
    Color separatorColor = (Color){54, 68, 82, 255};
    DrawRectangleRec(layout->turnRect, (Color){33, 43, 51, 255});
    DrawRectangleLinesEx(layout->turnRect, 1.0f, separatorColor);

    Color activeAlertColor = (match->activePlayerIdx == 0) ? (Color){0, 220, 255, 255} : (Color){255, 180, 0, 255};
    const char *activeTurnStr = (match->activePlayerIdx == 0) ? "PLAYER 1" : "PLAYER 2";

    int turnFontSize = (int)(baseFontSize * 1.6f);
    float indicatorTextY = layout->turnRect.y + (layout->turnRect.height - turnFontSize) / 2.0f;

    DrawRectangle(layout->turnRect.x + 2, layout->turnRect.y + 2, 8, layout->turnRect.height - 4, activeAlertColor);
    DrawAppText("TURN:", layout->turnRect.x + 22.0f, indicatorTextY, turnFontSize, (Color){150, 165, 175, 255});
    DrawAppText(activeTurnStr, layout->turnRect.x + 22.0f + MeasureAppText("TURN: ", turnFontSize), indicatorTextY, turnFontSize, activeAlertColor);

    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, layout->submitBtnRect);

    float pulse = (sinf((float)GetTime() * 4.0f) + 1.0f) * 0.5f;
    unsigned char outerAlpha = (unsigned char)(40 + pulse * 35);
    unsigned char fillAlpha = isHovered ? 230 : 200;

    Color neonGreenGlow = (Color){57, 255, 20, outerAlpha};
    Color neonGreenFill = (Color){20, 120, 40, fillAlpha};
    Color neonGreenBorder = isHovered ? (Color){120, 255, 90, 255} : (Color){57, 255, 20, 255};

    Rectangle outerGlow1 = {layout->submitBtnRect.x - 3, layout->submitBtnRect.y - 3, layout->submitBtnRect.width + 6, layout->submitBtnRect.height + 6};
    Rectangle outerGlow2 = {layout->submitBtnRect.x - 1, layout->submitBtnRect.y - 1, layout->submitBtnRect.width + 2, layout->submitBtnRect.height + 2};
    DrawRectangleRounded(outerGlow1, 0.35f, 6, neonGreenGlow);
    DrawRectangleRounded(outerGlow2, 0.35f, 6, neonGreenGlow);
    DrawRectangleRounded(layout->submitBtnRect, 0.35f, 6, neonGreenFill);
    DrawRectangleRoundedLinesEx(layout->submitBtnRect, 0.35f, 6, 2.0f, neonGreenBorder);

    const char *submitText = "SUBMIT";
    int submitFontSize = (int)(baseFontSize * 1.15f);
    int submitTextW = MeasureAppText(submitText, submitFontSize);
    float submitTextX = layout->submitBtnRect.x + (layout->submitBtnRect.width - submitTextW) / 2.0f;
    float submitTextY = layout->submitBtnRect.y + (layout->submitBtnRect.height - submitFontSize) / 2.0f;

    Color submitTextColor = isHovered ? (Color){255, 255, 255, 255} : (Color){220, 255, 210, 255};
    DrawAppText(submitText, submitTextX, submitTextY, submitFontSize, submitTextColor);

    if (GuiButton(layout->actionBtnRect, "Exit Match") && !match->shuffleState.isActive)
    {
        match->saveExitState.isActive = true;
        match->saveExitState.showFileDialog = false;
        PlaySoundEffect(SFX_BUTTON);
    }
}