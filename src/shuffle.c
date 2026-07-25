#include "shuffle.h"
#include "ui.h"
#include "sound.h"
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// State helpers
// ---------------------------------------------------------------------------

void ShuffleOpen(ShuffleState *shuffle)
{
    if (!shuffle) return;
    shuffle->isActive      = true;
    shuffle->selectedCount = 0;
    memset(shuffle->selected, 0, sizeof(shuffle->selected));
}

void ShuffleCancel(ShuffleState *shuffle)
{
    if (!shuffle) return;
    shuffle->isActive      = false;
    shuffle->selectedCount = 0;
    memset(shuffle->selected, 0, sizeof(shuffle->selected));
    PlaySoundEffect(SFX_BACK_NAV);
}

bool ShuffleCommit(ShuffleState *shuffle, Player *player, TileBag *bag, int *bagCountOut)
{
    if (!shuffle || !player || !bag) return false;
    if (shuffle->selectedCount <= 0)
    {
        // Nothing selected – just close
        ShuffleCancel(shuffle);
        return true;
    }
    // We need at least as many tiles in the bag as we're returning
    if (bag->tiles_remaining < shuffle->selectedCount)
        return false;

    // 1. Put selected tiles back into the bag
    for (int i = 0; i < player->rack_count; i++)
    {
        if (shuffle->selected[i])
        {
            if (bag->tiles_remaining < MAX_BAG_TILES)
            {
                bag->tiles[bag->tiles_remaining] = player->rack[i];
                bag->tiles_remaining++;
            }
            // Mark the rack slot as empty
            player->rack[i].letter   = '\0';
            player->rack[i].value    = 0;
            player->rack[i].isWildCard = false;
        }
    }

    // 2. Compact the rack (remove holes left by returned tiles)
    int writeIdx = 0;
    for (int i = 0; i < RACK_SIZE; i++)
    {
        if (player->rack[i].letter != '\0')
        {
            player->rack[writeIdx++] = player->rack[i];
        }
    }
    // Clear the rest
    for (int i = writeIdx; i < RACK_SIZE; i++)
    {
        player->rack[i].letter    = '\0';
        player->rack[i].value     = 0;
        player->rack[i].isWildCard = false;
    }
    player->rack_count = writeIdx;

    // 3. Reshuffle the entire bag so returned tiles are randomly reinserted
    shuffle_tiles_in_bag(bag);

    // 4. Refill the rack to 7 tiles from the freshly shuffled bag
    refill_rack(player, bag);

    // 5. Update the caller's bag count mirror
    if (bagCountOut) *bagCountOut = bag->tiles_remaining;

    PlaySoundEffect(SFX_TILE_PLACE);

    // Reset state
    shuffle->isActive      = false;
    shuffle->selectedCount = 0;
    memset(shuffle->selected, 0, sizeof(shuffle->selected));
    return true;
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

void ShuffleUpdate(ShuffleState *shuffle, const Player *player,
                   Rectangle rackRect, float tileSize, float tileSpacing)
{
    if (!shuffle || !shuffle->isActive || !player) return;

    // We draw tiles starting at rackRect.x + 15, centred vertically
    float tileY = rackRect.y + (rackRect.height - tileSize) / 2.0f + 4.0f;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        for (int t = 0; t < player->rack_count; t++)
        {
            Rectangle tileBounds = {
                rackRect.x + 15.0f + (t * (tileSize + tileSpacing)),
                tileY,
                tileSize,
                tileSize
            };
            if (CheckCollisionPointRec(mouse, tileBounds))
            {
                shuffle->selected[t] = !shuffle->selected[t];
                shuffle->selectedCount += shuffle->selected[t] ? 1 : -1;
                PlaySoundEffect(SFX_BUTTON);
                break;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------------

// Small helper – identical border color used throughout
static Color OverlayBorderColor(void) { return (Color){70, 90, 110, 220}; }

int ShuffleDraw(ShuffleState *shuffle, const Player *player,
                Rectangle rackRect, float tileSize, float tileSpacing,
                Rectangle confirmBtnRect, Rectangle cancelBtnRect,
                int baseFontSize)
{
    if (!shuffle || !shuffle->isActive || !player) return 0;

    // -----------------------------------------------------------------------
    // 1. Full-screen darkened overlay
    // -----------------------------------------------------------------------
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 175});

    // -----------------------------------------------------------------------
    // 2. Highlight / re-draw the rack region on top of the overlay
    // -----------------------------------------------------------------------
    // Soft glow border around the rack to indicate it's interactive
    Rectangle rackGlow = {rackRect.x - 3, rackRect.y - 3,
                          rackRect.width + 6, rackRect.height + 6};
    DrawRectangleRoundedLines(rackGlow, 0.15f, 6, (Color){80, 160, 255, 130});

    // Re-draw rack panel background (so tiles sit on a proper surface)
    DrawRectangleRec(rackRect, (Color){24, 32, 38, 255});
    DrawRectangleLinesEx(rackRect, 1.5f, OverlayBorderColor());

    // Header label
    const char *pickLbl = "Select tiles to exchange, then Confirm";
    int lblFontSize = (int)(baseFontSize * 0.85f);
    int lblW = MeasureAppText(pickLbl, lblFontSize);
    DrawAppText(pickLbl,
                rackRect.x + (rackRect.width - lblW) / 2.0f,
                rackRect.y + 5.0f,
                lblFontSize,
                (Color){170, 195, 220, 220});

    // -----------------------------------------------------------------------
    // 3. Draw each rack tile (highlight selected ones)
    // -----------------------------------------------------------------------
    float tileY = rackRect.y + (rackRect.height - tileSize) / 2.0f + 4.0f;

    for (int t = 0; t < player->rack_count; t++)
    {
        Tile tile = player->rack[t];
        Rectangle tileBounds = {
            rackRect.x + 15.0f + (t * (tileSize + tileSpacing)),
            tileY,
            tileSize,
            tileSize
        };

        bool sel = shuffle->selected[t];

        // Background: warm tile colour; selected gets a vivid blue tint
        Color faceBg    = sel ? (Color){80,  140, 230, 255} : (Color){244, 228, 198, 255};
        Color faceBorder = sel ? (Color){120, 200, 255, 255} : (Color){194, 169, 126, 255};

        // Glow ring when selected
        if (sel)
        {
            Rectangle glow = {tileBounds.x - 3, tileBounds.y - 3,
                              tileBounds.width + 6, tileBounds.height + 6};
            DrawRectangleRounded(glow, 0.25f, 4, (Color){100, 180, 255, 90});
        }

        DrawRectangleRounded(tileBounds, 0.2f, 4, faceBg);
        DrawRectangleRoundedLines(tileBounds, 0.2f, 4, faceBorder);

        char letterStr[2] = {tile.letter, '\0'};
        int tileFontSize  = (int)(tileSize * 0.55f);
        Color letterColor = sel ? (Color){255, 255, 255, 255} : (Color){38, 28, 16, 255};
        DrawAppText(letterStr,
                    tileBounds.x + (tileSize * 0.15f),
                    tileBounds.y + (tileSize - tileFontSize) / 2.0f,
                    tileFontSize, letterColor);

        int scoreFontSize = (int)(tileSize * 0.38f);
        const char *scoreStr = TextFormat("%d", tile.value);
        Color scoreColor = sel ? (Color){220, 240, 255, 200} : (Color){80, 65, 50, 255};
        DrawAppText(scoreStr,
                    tileBounds.x + tileSize - MeasureAppText(scoreStr, scoreFontSize) - (tileSize * 0.12f),
                    tileBounds.y + tileSize - scoreFontSize - (tileSize * 0.10f),
                    scoreFontSize, scoreColor);

        // Checkmark badge on selected tiles
        if (sel)
        {
            float badgeR  = tileSize * 0.16f;
            float badgeCX = tileBounds.x + tileSize - badgeR - 2;
            float badgeCY = tileBounds.y + badgeR + 2;
            DrawCircle((int)badgeCX, (int)badgeCY, badgeR, (Color){30, 220, 120, 255});
            // Simple tick – two lines
            DrawLineEx((Vector2){badgeCX - badgeR * 0.55f, badgeCY},
                       (Vector2){badgeCX - badgeR * 0.15f, badgeCY + badgeR * 0.45f},
                       1.5f, WHITE);
            DrawLineEx((Vector2){badgeCX - badgeR * 0.15f, badgeCY + badgeR * 0.45f},
                       (Vector2){badgeCX + badgeR * 0.55f, badgeCY - badgeR * 0.35f},
                       1.5f, WHITE);
        }
    }

    // -----------------------------------------------------------------------
    // 4. Confirm button  (green – same area as the former Pass/Shuffle btns)
    // -----------------------------------------------------------------------
    int result = 0;

    // Determine hover state
    Vector2 mouse = GetMousePosition();
    bool hoverConfirm = CheckCollisionPointRec(mouse, confirmBtnRect);
    bool hoverCancel  = CheckCollisionPointRec(mouse, cancelBtnRect);

    // --- Confirm ---
    Color confirmBg     = hoverConfirm ? (Color){30, 160,  70, 240} : (Color){20, 100, 50, 220};
    Color confirmBorder = hoverConfirm ? (Color){80, 255, 130, 255} : (Color){40, 180,  90, 255};
    bool confirmEnabled = (shuffle->selectedCount > 0);

    if (!confirmEnabled)
    {
        confirmBg     = (Color){30, 50, 40, 160};
        confirmBorder = (Color){50, 80, 60, 150};
    }

    DrawRectangleRounded(confirmBtnRect, 0.35f, 6, confirmBg);
    DrawRectangleRoundedLinesEx(confirmBtnRect, 0.35f, 6, 1.5f, confirmBorder);

    const char *confirmLabel = TextFormat("Confirm (%d)", shuffle->selectedCount);
    int cfsFontSize = (int)(baseFontSize * 0.9f);
    int cfsW = MeasureAppText(confirmLabel, cfsFontSize);
    Color confirmTextCol = confirmEnabled
        ? (hoverConfirm ? WHITE : (Color){200, 255, 215, 255})
        : (Color){90, 120, 100, 180};
    DrawAppText(confirmLabel,
                confirmBtnRect.x + (confirmBtnRect.width  - cfsW) / 2.0f,
                confirmBtnRect.y + (confirmBtnRect.height - cfsFontSize) / 2.0f,
                cfsFontSize, confirmTextCol);

    if (hoverConfirm && confirmEnabled && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        result = 1;

    // --- Cancel ---
    Color cancelBg     = hoverCancel ? (Color){140, 40, 40, 240} : (Color){80, 28, 28, 220};
    Color cancelBorder = hoverCancel ? (Color){255, 100, 90, 255} : (Color){180, 60, 55, 255};

    DrawRectangleRounded(cancelBtnRect, 0.35f, 6, cancelBg);
    DrawRectangleRoundedLinesEx(cancelBtnRect, 0.35f, 6, 1.5f, cancelBorder);

    const char *cancelLabel = "Cancel";
    int cclFontSize = (int)(baseFontSize * 0.9f);
    int cclW = MeasureAppText(cancelLabel, cclFontSize);
    Color cancelTextCol = hoverCancel ? WHITE : (Color){255, 190, 185, 255};
    DrawAppText(cancelLabel,
                cancelBtnRect.x + (cancelBtnRect.width  - cclW) / 2.0f,
                cancelBtnRect.y + (cancelBtnRect.height - cclFontSize) / 2.0f,
                cclFontSize, cancelTextCol);

    if (hoverCancel && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        result = -1;

    return result;
}
