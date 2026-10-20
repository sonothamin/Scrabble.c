#include "game_actions.h"
#include "sound.h"
#include "word_validation.h"
#include "error_service.h"
#include <stdio.h>
#include <string.h>

void Action_ReturnStagedTilesToRack(GameState *match)
{
    int ap = match->activePlayerIdx;
    for (int uy = 0; uy < BOARD_SIDE; uy++)
    {
        for (int ux = 0; ux < BOARD_SIDE; ux++)
        {
            if (match->board.grid[uy][ux].letter != '\0' &&
                match->previousBoard.grid[uy][ux].letter == '\0' &&
                match->players[ap].rack_count < RACK_SIZE)
            {
                match->players[ap].rack[match->players[ap].rack_count] =
                    WildTileAsRackTile(match->board.grid[uy][ux]);
                match->players[ap].rack_count++;
            }
        }
    }
}

void Action_FinalizeGameOver(GameState *match)
{
    if (!match)
        return;
    PlaySoundEffect(SFX_GAME_OVER);
    match->isMatchOver = true;

    int rackPenalty[2] = {0, 0};
    for (int p = 0; p < 2; p++)
    {
        for (int i = 0; i < match->players[p].rack_count; i++)
        {
            if (match->players[p].rack[i].letter != '\0')
            {
                rackPenalty[p] += match->players[p].rack[i].value;
            }
        }
    }

    for (int p = 0; p < 2; p++)
    {
        match->players[p].score -= rackPenalty[p];
        if (match->players[p].score < 0)
        {
            match->players[p].score = 0;
        }
    }

    for (int p = 0; p < 2; p++)
    {
        if (match->players[p].rack_count == 0)
        {
            int opponentIdx = (p + 1) % 2;
            match->players[p].score += rackPenalty[opponentIdx];
        }
    }

    if (match->players[0].score > match->players[1].score)
    {
        match->winningPlayerIdx = 0;
    }
    else if (match->players[1].score > match->players[0].score)
    {
        match->winningPlayerIdx = 1;
    }
    else if (match->players[1].score == match->players[0].score)
    {
        match->winningPlayerIdx = 2;
        match->isTie = true;
    }
}

void Action_PassTurn(GameState *match)
{
    Action_ReturnStagedTilesToRack(match);
    memcpy(&match->board, &match->previousBoard, sizeof(GameBoard));
    match->consecutivePassCount++;
    PlaySoundEffect(SFX_BACK_NAV);

    if (match->consecutivePassCount >= 6)
    {
        Action_FinalizeGameOver(match);
    }
    else
    {
        match->activePlayerIdx = (match->activePlayerIdx + 1) % 2;
    }
}

void Action_UndoTurn(GameState *match)
{
    if (Action_HasStagedTiles(match))
    {
        Action_ReturnStagedTilesToRack(match);
        memcpy(&match->board, &match->previousBoard, sizeof(GameBoard));
        PlaySoundEffect(SFX_BACK_NAV);
    }
}

bool Action_HasStagedTiles(const GameState *match)
{
    for (int uy = 0; uy < BOARD_SIDE; uy++)
    {
        for (int ux = 0; ux < BOARD_SIDE; ux++)
        {
            if (match->board.grid[uy][ux].letter != '\0' &&
                match->previousBoard.grid[uy][ux].letter == '\0')
            {
                return true;
            }
        }
    }
    return false;
}

void Action_SubmitMove(GameState *match)
{
    if (WildTileHasUnassignedOnBoard(&match->board, &match->previousBoard))
    {
        ReportGameWarning("Wildcard Unassigned", "Choose a letter for every blank tile before submitting.", 3.0f);
        PlaySoundEffect(SFX_INVALID_MOVE);
        return;
    }

    ScoreBonusKind bestBonus = SCORE_BONUS_NONE;
    int scoreGain = Scan_And_Validate_Move(
        match->board.grid,
        match->previousBoard.grid,
        match->board.cells,
        match->specialTilesEnabled,
        &match->dictionary,
        &bestBonus);

    if (scoreGain > 0)
    {
        // --- RECORD FULL FORMED WORD & TILES INTO HISTORY ---
        if (match->historyCount < MAX_HISTORY_ENTRIES)
        {
            TurnHistoryEntry *hist = &match->historyEntries[match->historyCount++];
            hist->playerIdx = match->activePlayerIdx;
            hist->scoreEarned = scoreGain;
            hist->bonus = bestBonus;

            int minX = BOARD_SIDE, maxX = -1;
            int minY = BOARD_SIDE, maxY = -1;

            // 1. Locate newly placed tiles bounds
            for (int y = 0; y < BOARD_SIDE; y++)
            {
                for (int x = 0; x < BOARD_SIDE; x++)
                {
                    if (match->board.grid[y][x].letter != '\0' &&
                        match->previousBoard.grid[y][x].letter == '\0')
                    {
                        if (x < minX)
                            minX = x;
                        if (x > maxX)
                            maxX = x;
                        if (y < minY)
                            minY = y;
                        if (y > maxY)
                            maxY = y;
                    }
                }
            }

            int startX = minX, endX = maxX;
            int startY = minY, endY = maxY;
            bool isHorizontal = (minY == maxY);

            // 2. Expand outwards to include attached existing letters
            if (isHorizontal)
            {
                while (startX > 0 && match->board.grid[startY][startX - 1].letter != '\0')
                    startX--;
                while (endX < BOARD_SIDE - 1 && match->board.grid[endY][endX + 1].letter != '\0')
                    endX++;
            }
            else
            {
                while (startY > 0 && match->board.grid[startY - 1][startX].letter != '\0')
                    startY--;
                while (endY < BOARD_SIDE - 1 && match->board.grid[endY + 1][startX].letter != '\0')
                    endY++;
            }

            // 3. Traverse in sequence to build the complete word & tile info
            int count = 0;
            if (isHorizontal)
            {
                for (int x = startX; x <= endX && count < BOARD_SIDE; x++)
                {
                    hist->word[count] = match->board.grid[startY][x].letter;
                    hist->tiles[count].letter = match->board.grid[startY][x].letter;
                    hist->tiles[count].value = match->board.grid[startY][x].value;
                    hist->tiles[count].luxury = match->board.cells[startY][x];
                    count++;
                }
            }
            else
            {
                for (int y = startY; y <= endY && count < BOARD_SIDE; y++)
                {
                    hist->word[count] = match->board.grid[y][startX].letter;
                    hist->tiles[count].letter = match->board.grid[y][startX].letter;
                    hist->tiles[count].value = match->board.grid[y][startX].value;
                    hist->tiles[count].luxury = match->board.cells[y][startX];
                    count++;
                }
            }
            hist->word[count] = '\0';
            hist->tileCount = count;
        }

        if (bestBonus == SCORE_BONUS_3W)
            PlaySoundEffect(SFX_SCORE_3W);
        else if (bestBonus == SCORE_BONUS_2W)
            PlaySoundEffect(SFX_SCORE_2W);
        else
            PlaySoundEffect(SFX_SCORE);

        match->players[match->activePlayerIdx].score += scoreGain;
        refill_rack(&match->players[match->activePlayerIdx], &match->tileBag);
        match->tileBagCount = match->tileBag.tiles_remaining;
        match->consecutivePassCount = 0;

        memcpy(&match->previousBoard, &match->board, sizeof(GameBoard));

        if (match->tileBagCount == 0 && match->players[match->activePlayerIdx].rack_count == 0)
        {
            Action_FinalizeGameOver(match);
        }
        else
        {
            match->activePlayerIdx = (match->activePlayerIdx + 1) % 2;
        }
    }
}