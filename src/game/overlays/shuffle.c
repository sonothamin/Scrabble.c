#include "shuffle.h"
#include "ui.h"
#include "sound.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ---------------------------------------------------------------------------
// Internal layout helper – computes modal geometry from screen dimensions.
// All drawing and hit-testing derives from this single function.
// ---------------------------------------------------------------------------

typedef struct
{
    Rectangle modal;        // outer modal panel
    Rectangle tilesArea;    // inner area where tiles are drawn
    float     tileSize;
    float     tileSpacing;
    float     tileRowY;     // Y of the tile row (top of tiles)
    Rectangle confirmBtn;
    Rectangle cancelBtn;
    int       titleFontSize;
    int       subFontSize;
    int       tileFontSize;
} ShuffleLayout;

static ShuffleLayout ComputeLayout(int screenW, int screenH, int baseFontSize)
{
    ShuffleLayout L = {0};

    // Modal dimensions – responsive but capped for large screens
    float modalW = fminf(screenW * 0.60f, 700.0f);
    float modalH = fminf(screenH * 0.52f, 440.0f);
    float modalX = (screenW - modalW) / 2.0f;
    float modalY = (screenH - modalH) / 2.0f;
    L.modal = (Rectangle){modalX, modalY, modalW, modalH};

    float pad = 18.0f;

    // Fonts
    L.titleFontSize = (int)(baseFontSize * 1.15f);
    L.subFontSize   = (int)(baseFontSize * 0.82f);

    // Tile row – sits in the upper half of the modal body
    float bodyTop    = modalY + pad + L.titleFontSize + 10.0f + L.subFontSize + 10.0f;
    float bodyBottom = modalY + modalH - pad - 44.0f - 10.0f; // leave room for buttons
    float tileRowH   = bodyBottom - bodyTop;

    L.tileSize    = fminf(tileRowH * 0.80f, modalW / 9.5f);
    L.tileSpacing = fminf(10.0f, (modalW - pad * 2.0f - RACK_SIZE * L.tileSize) / (RACK_SIZE - 1));
    L.tileFontSize = (int)(L.tileSize * 0.55f);

    // Centre the tile row horizontally
    float rowWidth = RACK_SIZE * L.tileSize + (RACK_SIZE - 1) * L.tileSpacing;
    float rowStartX = modalX + (modalW - rowWidth) / 2.0f;
    L.tileRowY = bodyTop + (tileRowH - L.tileSize) / 2.0f;
    L.tilesArea = (Rectangle){rowStartX, L.tileRowY, rowWidth, L.tileSize};

    // Hotkey bar sits at the very bottom of the modal
    float hkBarH   = (float)(int)(baseFontSize * 0.72f) * 2.0f + 6.0f;  // matches DrawHotkeyBar sizing
    float hkBarPad = 8.0f;  // gap between hotkey bar and modal bottom edge
    float hkBarY   = modalY + modalH - hkBarPad - hkBarH;

    // Buttons sit immediately above the hotkey bar
    float btnW   = modalW * 0.38f;
    float btnH   = 40.0f;
    float btnGap = 12.0f;                       // fixed narrow gap between the two buttons
    float btnBlockW = btnW * 2.0f + btnGap;
    float btnStartX = modalX + (modalW - btnBlockW) / 2.0f;  // centred
    float btnY   = hkBarY - 10.0f - btnH;      // 10px breathing room above hotkey bar

    L.confirmBtn = (Rectangle){btnStartX,             btnY, btnW, btnH};
    L.cancelBtn  = (Rectangle){btnStartX + btnW + btnGap, btnY, btnW, btnH};

    return L;
}

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
        ShuffleCancel(shuffle);
        return true;
    }
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
            player->rack[i].letter    = '\0';
            player->rack[i].value     = 0;
            player->rack[i].isWildCard = false;
        }
    }

    // 2. Compact the rack
    int writeIdx = 0;
    for (int i = 0; i < RACK_SIZE; i++)
    {
        if (player->rack[i].letter != '\0')
            player->rack[writeIdx++] = player->rack[i];
    }
    for (int i = writeIdx; i < RACK_SIZE; i++)
    {
        player->rack[i].letter    = '\0';
        player->rack[i].value     = 0;
        player->rack[i].isWildCard = false;
    }
    player->rack_count = writeIdx;

    // 3. Reshuffle bag and refill rack
    shuffle_tiles_in_bag(bag);
    refill_rack(player, bag);

    if (bagCountOut) *bagCountOut = bag->tiles_remaining;

    PlaySoundEffect(SFX_TILE_PLACE);

    shuffle->isActive      = false;
    shuffle->selectedCount = 0;
    memset(shuffle->selected, 0, sizeof(shuffle->selected));
    return true;
}

// ---------------------------------------------------------------------------
// Input  (modal-space hit-testing, fully independent of game layout)
// ---------------------------------------------------------------------------

void ShuffleUpdate(ShuffleState *shuffle, const Player *player,
                   int screenW, int screenH, int baseFontSize)
{
    if (!shuffle || !shuffle->isActive || !player) return;

    ShuffleLayout L = ComputeLayout(screenW, screenH, baseFontSize);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        for (int t = 0; t < player->rack_count; t++)
        {
            Rectangle tileBounds = {
                L.tilesArea.x + t * (L.tileSize + L.tileSpacing),
                L.tileRowY,
                L.tileSize,
                L.tileSize
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
// Drawing  (fully self-contained modal)
// ---------------------------------------------------------------------------

// Accent palette
static Color ColAccentBlue(void)   { return (Color){100, 185, 255, 255}; }
static Color ColModalBg(void)      { return (Color){ 18,  24,  32, 252}; }
static Color ColModalBorder(void)  { return (Color){ 60,  85, 115, 220}; }
static Color ColSubtleText(void)   { return (Color){140, 160, 185, 210}; }

int ShuffleDraw(ShuffleState *shuffle, const Player *player,
                int screenW, int screenH, int baseFontSize)
{
    if (!shuffle || !shuffle->isActive || !player) return 0;

    ShuffleLayout L = ComputeLayout(screenW, screenH, baseFontSize);
    Vector2 mouse   = GetMousePosition();
    int result      = 0;

    // -------------------------------------------------------------------
    // 1. Full-screen scrim
    // -------------------------------------------------------------------
    DrawRectangle(0, 0, screenW, screenH, (Color){0, 0, 0, 190});

    // -------------------------------------------------------------------
    // 2. Modal panel  (layered for depth)
    // -------------------------------------------------------------------
    // Outer glow
    Rectangle glow1 = {L.modal.x - 6, L.modal.y - 6, L.modal.width + 12, L.modal.height + 12};
    Rectangle glow2 = {L.modal.x - 2, L.modal.y - 2, L.modal.width +  4, L.modal.height +  4};
    DrawRectangleRounded(glow1, 0.10f, 8, (Color){ 80, 150, 255, 30});
    DrawRectangleRounded(glow2, 0.10f, 8, (Color){ 80, 150, 255, 55});

    // Panel background
    DrawRectangleRounded(L.modal, 0.08f, 8, ColModalBg());

    // Panel border
    DrawRectangleRoundedLinesEx(L.modal, 0.08f, 8, 1.5f, ColModalBorder());

    // -------------------------------------------------------------------
    // 3. Header text
    // -------------------------------------------------------------------
    float pad  = 18.0f;
    float textX = L.modal.x + pad;
    float textY = L.modal.y + pad + 6.0f;

    // Title
    const char *title = "Exchange Tiles";
    DrawAppText(title, textX, textY, L.titleFontSize, WHITE);

    // Sub-label
    float subY = textY + L.titleFontSize + 6.0f;
    const char *subLabel = (shuffle->selectedCount == 0)
        ? "Click tiles to mark them for exchange"
        : TextFormat("%d tile%s selected — confirm to swap from the bag",
                     shuffle->selectedCount, shuffle->selectedCount == 1 ? "" : "s");
    DrawAppText(subLabel, textX, subY, L.subFontSize, ColSubtleText());

    // Thin separator under header
    float sepY = subY + L.subFontSize + 8.0f;
    DrawLineEx((Vector2){L.modal.x + pad, sepY},
               (Vector2){L.modal.x + L.modal.width - pad, sepY},
               1.0f, ColModalBorder());

    // -------------------------------------------------------------------
    // 4. Tile row
    // -------------------------------------------------------------------
    for (int t = 0; t < player->rack_count; t++)
    {
        Tile tile = player->rack[t];
        Rectangle tileBounds = {
            L.tilesArea.x + t * (L.tileSize + L.tileSpacing),
            L.tileRowY,
            L.tileSize,
            L.tileSize
        };

        bool sel  = shuffle->selected[t];
        bool hovT = CheckCollisionPointRec(mouse, tileBounds);

        // Outer glow for selected or hovered
        if (sel || hovT)
        {
            Color glowCol = sel
                ? (Color){100, 190, 255, 70}
                : (Color){200, 200, 200, 35};
            Rectangle gr = {tileBounds.x - 4, tileBounds.y - 4,
                            tileBounds.width + 8, tileBounds.height + 8};
            DrawRectangleRounded(gr, 0.28f, 4, glowCol);
        }

        // Face colour
        Color faceBg     = sel ? (Color){ 65, 140, 240, 255} : (Color){244, 228, 198, 255};
        Color faceBorder = sel ? (Color){140, 210, 255, 255} : (Color){194, 169, 126, 255};

        // Hover tint when not selected
        if (!sel && hovT)
        {
            faceBg     = (Color){250, 238, 215, 255};
            faceBorder = (Color){210, 190, 148, 255};
        }

        DrawRectangleRounded(tileBounds, 0.22f, 4, faceBg);
        DrawRectangleRoundedLines(tileBounds, 0.22f, 4, faceBorder);

        // Letter
        char letterStr[2] = {tile.letter, '\0'};
        Color letterColor = sel ? WHITE : (Color){38, 28, 16, 255};
        DrawAppText(letterStr,
                    tileBounds.x + (L.tileSize * 0.15f),
                    tileBounds.y + (L.tileSize - L.tileFontSize) / 2.0f,
                    L.tileFontSize, letterColor);

        // Score subscript
        int scoreFontSize = (int)(L.tileSize * 0.36f);
        const char *scoreStr = TextFormat("%d", tile.value);
        Color scoreColor = sel ? (Color){210, 235, 255, 210} : (Color){80, 65, 50, 255};
        DrawAppText(scoreStr,
                    tileBounds.x + L.tileSize - MeasureAppText(scoreStr, scoreFontSize) - (L.tileSize * 0.10f),
                    tileBounds.y + L.tileSize - scoreFontSize - (L.tileSize * 0.08f),
                    scoreFontSize, scoreColor);

        // Checkmark badge (selected)
        if (sel)
        {
            float bR  = L.tileSize * 0.16f;
            float bCX = tileBounds.x + L.tileSize - bR - 2.0f;
            float bCY = tileBounds.y + bR + 2.0f;
            DrawCircle((int)bCX, (int)bCY, bR, (Color){30, 220, 120, 255});
            DrawLineEx((Vector2){bCX - bR * 0.55f, bCY},
                       (Vector2){bCX - bR * 0.15f, bCY + bR * 0.45f},
                       1.5f, WHITE);
            DrawLineEx((Vector2){bCX - bR * 0.15f, bCY + bR * 0.45f},
                       (Vector2){bCX + bR * 0.55f, bCY - bR * 0.35f},
                       1.5f, WHITE);
        }
    }

    // -------------------------------------------------------------------
    // 5. Confirm button
    // -------------------------------------------------------------------
    bool confirmEnabled = (shuffle->selectedCount > 0);
    bool hoverConfirm   = CheckCollisionPointRec(mouse, L.confirmBtn);

    Color confirmBg, confirmBorder, confirmText;
    if (!confirmEnabled)
    {
        confirmBg     = (Color){ 25, 40, 35, 160};
        confirmBorder = (Color){ 45, 65, 55, 140};
        confirmText   = (Color){ 80, 100, 90, 160};
    }
    else if (hoverConfirm)
    {
        confirmBg     = (Color){ 35, 175, 80, 245};
        confirmBorder = (Color){ 90, 255, 140, 255};
        confirmText   = WHITE;
    }
    else
    {
        confirmBg     = (Color){ 22, 110, 55, 220};
        confirmBorder = (Color){ 45, 190, 100, 255};
        confirmText   = (Color){200, 255, 215, 255};
    }

    DrawRectangleRounded(L.confirmBtn, 0.35f, 6, confirmBg);
    DrawRectangleRoundedLinesEx(L.confirmBtn, 0.35f, 6, 1.5f, confirmBorder);

    const char *confirmLabel = TextFormat("Exchange (%d)", shuffle->selectedCount);
    int cfsFontSize = (int)(baseFontSize * 0.90f);
    int cfsW = MeasureAppText(confirmLabel, cfsFontSize);
    DrawAppText(confirmLabel,
                L.confirmBtn.x + (L.confirmBtn.width  - cfsW) / 2.0f,
                L.confirmBtn.y + (L.confirmBtn.height - cfsFontSize) / 2.0f,
                cfsFontSize, confirmText);

    if (hoverConfirm && confirmEnabled && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        result = 1;

    // -------------------------------------------------------------------
    // 6. Cancel button
    // -------------------------------------------------------------------
    bool hoverCancel = CheckCollisionPointRec(mouse, L.cancelBtn);

    Color cancelBg     = hoverCancel ? (Color){155, 42, 42, 245} : (Color){ 80, 28, 28, 220};
    Color cancelBorder = hoverCancel ? (Color){255, 105, 95, 255} : (Color){180, 60, 55, 255};
    Color cancelText   = hoverCancel ? WHITE : (Color){255, 185, 180, 255};

    DrawRectangleRounded(L.cancelBtn, 0.35f, 6, cancelBg);
    DrawRectangleRoundedLinesEx(L.cancelBtn, 0.35f, 6, 1.5f, cancelBorder);

    const char *cancelLabel = "Cancel";
    int cclFontSize = (int)(baseFontSize * 0.90f);
    int cclW = MeasureAppText(cancelLabel, cclFontSize);
    DrawAppText(cancelLabel,
                L.cancelBtn.x + (L.cancelBtn.width  - cclW) / 2.0f,
                L.cancelBtn.y + (L.cancelBtn.height - cclFontSize) / 2.0f,
                cclFontSize, cancelText);

    if (hoverCancel && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        result = -1;

    // -------------------------------------------------------------------
    // 7. Hotkey bar  (ESC = Cancel, C = Confirm)
    // -------------------------------------------------------------------
    float hkBarH   = (float)(int)(baseFontSize * 0.72f) * 2.0f + 6.0f;
    float hkBarPad = 8.0f;
    float hkBarY   = L.modal.y + L.modal.height - hkBarPad - hkBarH;
    float hkBarX   = L.modal.x + 18.0f;
    float hkBarW   = L.modal.width - 36.0f;

    HotkeyEntry shuffleKeys[] = {
        { "ESC", "Cancel"  },
        { "C",   "Confirm" },
    };
    DrawHotkeyBar(shuffleKeys, 2, hkBarX, hkBarY, hkBarW, hkBarH, (float)baseFontSize);

    return result;
}
