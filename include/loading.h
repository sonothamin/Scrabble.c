// Sonoth
#ifndef LOADING_H
#define LOADING_H

#include "app_state.h"

typedef struct LoadingState {
    float progress;          
    float tileTimers[10];    
    float tileYOffsets[10];  
    float vfxRotation;       
    float particlePulse;     
} LoadingState;

void LoadingInit(LoadingState* lState);
void LoadingUpdate(AppState* state, LoadingState* lState, float deltaTime);
void LoadingDraw(AppState* state, LoadingState* lState);

#endif