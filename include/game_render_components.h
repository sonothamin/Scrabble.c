#ifndef GAME_RENDER_COMPONENTS_H
#define GAME_RENDER_COMPONENTS_H

#include "game.h"
#include "app_state.h"
#include "game_layout.h"

void Draw_BoardView(const GameState *match, const GameLayout *layout);
void Draw_HotkeyView(const AppState *state, const GameLayout *layout, int baseFontSize);
void Draw_MatchStatusView(const GameState *match, const GameLayout *layout);
void Draw_ScoreBoardView(const GameState *match, const GameLayout *layout, int baseFontSize);
void Draw_RackView(const GameState *match, const GameLayout *layout, int baseFontSize);
void Draw_ActionBarView(GameState *match, const GameLayout *layout, int baseFontSize);
void Draw_HistoryView(const GameLayout *layout, int baseFontSize);
void Draw_FooterView(GameState *match, const GameLayout *layout, int baseFontSize);

#endif