#ifndef ABOUT_H
#define ABOUT_H
#include "app_state.h"

// Tracks which tab is currently selected on the About screen
typedef enum {
    ABOUT_TAB_OVERVIEW = 0,
    ABOUT_TAB_TEAM,
    ABOUT_TAB_TECH
} AboutTab;

typedef struct AboutState{
    AboutTab activeTab;
    float scrollOffset; // For text-heavy sections if needed
} AboutState;

void AboutInit(AboutState* state);
void AboutUpdate(AppState* appState, AboutState* aboutState);
void AboutDraw(AppState* appState, AboutState* aboutState);

#endif // ABOUT_H