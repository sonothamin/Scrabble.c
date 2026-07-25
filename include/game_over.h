#pragma once

#include "app_state.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct GameOverState {
    float animTime;
} GameOverState;

void InitGameOverState(GameOverState *gameOver);
void UpdateGameOverOverlay(AppState *state, GameOverState *gameOver);
void DrawGameOverOverlay(AppState *state, GameOverState *gameOver);

#if defined(__cplusplus)
}
#endif
