#pragma once

#include "app_state.h"

#if defined(__cplusplus)
extern "C" {
#endif

void GameUpdate(AppState* state);
void GameDraw(AppState* state);

#if defined(__cplusplus)
}
#endif
