#pragma once

#include "app_state.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct PauseState {
    bool isPaused;
} PauseState;

void InitPauseState(PauseState *pause);
void UpdatePauseOverlay(AppState *state, PauseState *pause);
void DrawPauseOverlay(AppState *state, PauseState *pause);

#if defined(__cplusplus)
}
#endif
