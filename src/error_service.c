#include "error_service.h"
#include <string.h>
#include "raylib.h"
#include "raygui.h"
#include "ui.h"

static ErrorState g_ErrorState = { .isError = false, .title = "All Systems Go", .message = "No Error. Yaaay!" };

void ReportCriticalError(const char* title, const char* message)
{
    g_ErrorState.isError = true;
    
    strncpy(g_ErrorState.title, title, sizeof(g_ErrorState.title) - 1);
    strncpy(g_ErrorState.message, message, sizeof(g_ErrorState.message) - 1);
    
    TraceLog(LOG_ERROR, "[CRITICAL ERROR] %s: %s", title, message);
}

bool HasGlobalError(void)
{
    return g_ErrorState.isError;
}

ErrorState* GetGlobalErrorState(void)
{
    return &g_ErrorState;
}

void ClearGlobalError(void)
{
    g_ErrorState.isError = false;
    strcpy(g_ErrorState.title, "All Systems Go");
    strcpy(g_ErrorState.message, "No Error. Yaaay!");
}

bool ShowErrorDialog(void)
{
    if (!g_ErrorState.isError) return false;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 130 });

    float paddingX = 50.0f;
    int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int textWidth = MeasureAppText(g_ErrorState.message, fontSize);

    float msgBoxWidth = textWidth + (paddingX * 2.0f);
    if (msgBoxWidth < 450.0f) msgBoxWidth = 450.0f; 
    
    float msgBoxHeight = 220.0f; 

    Rectangle bounds = {
        (float)(GetScreenWidth() - msgBoxWidth) / 2.0f,
        (float)(GetScreenHeight() - msgBoxHeight) / 2.0f,
        msgBoxWidth,
        msgBoxHeight
    };

    int result = GuiMessageBox(bounds, g_ErrorState.title, g_ErrorState.message, "Exit");
    
    if (result == 1)
    {
        return true;
    }

    return false;
}