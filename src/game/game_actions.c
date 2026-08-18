#include "game_actions.h"
#include "sound.h"
#include "word_validation.h"
#include "error_service.h"
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
    if (!match) return;

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
    else
    {
        match->winningPlayerIdx = 0;
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