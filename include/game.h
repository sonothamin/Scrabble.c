#pragma once

#include "app_state.h"

#if defined(__cplusplus)
extern "C" {
#endif

void GameUpdate(AppState* state);
void GameDraw(void);

#if defined(__cplusplus)
}
#endif
