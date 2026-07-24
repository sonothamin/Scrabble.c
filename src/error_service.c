#include "error_service.h"
#include <string.h>
#include "raylib.h"
#include "raygui.h"
#include "ui.h"

static ErrorState g_ErrorState = {.isError = false, .title = "All Systems Go", .message = "No Error. Yaaay!"};

// For NON-FATAL errors
static char g_warningTitle[64] = {0};
static char g_warningMessage[128] = {0};
static float g_warningTimer = 0.0f;

void ReportCriticalError(const char *title, const char *message)
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

ErrorState *GetGlobalErrorState(void)
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
    if (!g_ErrorState.isError)
        return false;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 130});

    float paddingX = 50.0f;
    int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int textWidth = MeasureAppText(g_ErrorState.message, fontSize);

    float msgBoxWidth = textWidth + (paddingX * 2.0f);
    if (msgBoxWidth < 450.0f)
        msgBoxWidth = 450.0f;

    float msgBoxHeight = 220.0f;

    Rectangle bounds = {
        (float)(GetScreenWidth() - msgBoxWidth) / 2.0f,
        (float)(GetScreenHeight() - msgBoxHeight) / 2.0f,
        msgBoxWidth,
        msgBoxHeight};

    int result = GuiMessageBox(bounds, g_ErrorState.title, g_ErrorState.message, "Exit");

    if (result == 1)
    {
        return true;
    }

    return false;
}

void ReportGameWarning(const char *title, const char *message, float duration)
{
    if (title)
    {
        strncpy(g_warningTitle, title, sizeof(g_warningTitle) - 1);
        g_warningTitle[sizeof(g_warningTitle) - 1] = '\0';
    }
    if (message)
    {
        strncpy(g_warningMessage, message, sizeof(g_warningMessage) - 1);
        g_warningMessage[sizeof(g_warningMessage) - 1] = '\0';
    }

    g_warningTimer = duration;
}

void UpdateErrorService(float deltaTime)
{
    if (g_warningTimer > 0.0f)
    {
        g_warningTimer -= deltaTime;
        if (g_warningTimer < 0.0f)
        {
            g_warningTimer = 0.0f;
        }
    }
}

void DrawErrorServiceOverlay(void)
{
    if (g_warningTimer <= 0.0f)
        return;

    const int screenWidth = GetScreenWidth();
    float bannerWidth = 420.0f;
    float bannerHeight = 50.0f;

    Rectangle bannerRect = {(screenWidth - bannerWidth) / 2.0f, 25.0f, bannerWidth, bannerHeight};
}