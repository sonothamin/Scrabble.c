#include "menu.h"
#include "ui.h"

typedef enum MenuAction
{
    MENU_ACTION_LOCAL_GAME = 0,
    MENU_ACTION_NETWORK_GAME,
    MENU_ACTION_LOAD_GAME,
    MENU_ACTION_SETTINGS,
    MENU_ACTION_EXIT,
    MENU_ACTION_COUNT
} MenuAction;

typedef struct MenuEntry
{
    const char* title;
    const char* description;
} MenuEntry;

static int selectedOption = 0;

static int ClampInt(int value, int minValue, int maxValue)
{
    if (value < minValue)
        return minValue;

    if (value > maxValue)
        return maxValue;

    return value;
}

static void ExecuteMenuSelection(AppState* state, int option)
{
    if (state == NULL)
        return;

    switch (option)
    {
        case MENU_ACTION_LOCAL_GAME:
            state->currentScreen = APP_SCREEN_GAME;
            break;

        case MENU_ACTION_NETWORK_GAME:
            state->currentScreen = APP_SCREEN_GAME;
            break;

        case MENU_ACTION_LOAD_GAME:
            state->currentScreen = APP_SCREEN_GAME;
            break;

        case MENU_ACTION_SETTINGS:
            state->currentScreen = APP_SCREEN_SETTINGS;
            break;

        case MENU_ACTION_EXIT:
            CloseWindow();
            break;

        default:
            break;
    }
}

static void DrawMenuButton(int x, int y, int width, int height, const MenuEntry* entry, bool selected)
{
    Color fillColor = selected ? Fade(ORANGE, 0.22f) : Fade(WHITE, 0.95f);
    Color outlineColor = selected ? ORANGE : LIGHTGRAY;
    Color titleColor = selected ? MAROON : DARKGRAY;

    DrawRectangle(x, y, width, height, fillColor);
    DrawRectangleLines(x, y, width, height, outlineColor);

    const int fontSize = (height >= 70) ? 22 : 18;
    const int titleWidth = MeasureText(entry->title, fontSize);
    const int titleX = x + (width - titleWidth) / 2;
    const int titleY = y + (height - fontSize) / 2;

    DrawText(entry->title, titleX, titleY, fontSize, titleColor);

    if (selected)
    {
        DrawText("->", x + 16, y + (height - 20) / 2, 20, ORANGE);
        DrawText("<-", x + width - 36, y + (height - 20) / 2, 20, ORANGE);
    }
}

void MenuUpdate(AppState* state)
{
    if (state == NULL)
        return;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = (selectedOption + 1) % MENU_ACTION_COUNT;
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = (selectedOption + MENU_ACTION_COUNT - 1) % MENU_ACTION_COUNT;
    }
    else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        ExecuteMenuSelection(state, selectedOption);
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const Vector2 mousePosition = GetMousePosition();

    const int gameButtonHeight = ClampInt(screenHeight / 10, 56, 72);
    const int gameButtonWidth = ClampInt((screenWidth - 140) / 3, 96, 160);
    const int gameGap = ClampInt((screenWidth - 3 * gameButtonWidth - 80) / 2, 14, 24);
    const int gameGroupY = ClampInt(screenHeight / 2 - 40, 180, 230);
    const int gameGroupStartX = (screenWidth - (3 * gameButtonWidth + 2 * gameGap)) / 2;

    for (int i = 0; i < 3; ++i)
    {
        Rectangle buttonRect = {
            (float)(gameGroupStartX + i * (gameButtonWidth + gameGap)),
            (float)gameGroupY,
            (float)gameButtonWidth,
            (float)gameButtonHeight
        };

        if (CheckCollisionPointRec(mousePosition, buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectedOption = i;
            ExecuteMenuSelection(state, selectedOption);
        }
    }

    const int actionWidth = ClampInt(screenWidth - 140, 240, 420);
    const int actionHeight = ClampInt(screenHeight / 10, 54, 64);
    const int actionX = (screenWidth - actionWidth) / 2;
    const int settingsY = ClampInt(gameGroupY + gameButtonHeight + 32, 300, 360);
    const int exitY = ClampInt(settingsY + actionHeight + 18, 380, 440);

    Rectangle settingsRect = {(float)actionX, (float)settingsY, (float)actionWidth, (float)actionHeight};
    Rectangle exitRect = {(float)actionX, (float)exitY, (float)actionWidth, (float)actionHeight};

    if (CheckCollisionPointRec(mousePosition, settingsRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        selectedOption = MENU_ACTION_SETTINGS;
        ExecuteMenuSelection(state, selectedOption);
    }

    if (CheckCollisionPointRec(mousePosition, exitRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        selectedOption = MENU_ACTION_EXIT;
        ExecuteMenuSelection(state, selectedOption);
    }
}

void MenuDraw(void)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const int titleY = 70;
    const int subtitleY = 112;
    const int titleWidth = MeasureText("SCRABBLE.C", 42);
    const int subtitleWidth = MeasureText("Choose your next move", 20);

    DrawText("SCRABBLE.C", (screenWidth - titleWidth) / 2, titleY, 42, DARKGRAY);
    DrawText("Choose your next move", (screenWidth - subtitleWidth) / 2, subtitleY, 20, GRAY);

    DrawLine(80, 150, screenWidth - 80, 150, LIGHTGRAY);

    static const MenuEntry gameEntries[3] = {
        {"Local", "Play a match locally"},
        {"Network", "Challenge another player"},
        {"Load", "Resume from a save file"}
    };

    static const MenuEntry settingsEntry = {"Settings", "Adjust visuals, sound, and rules"};
    static const MenuEntry exitEntry = {"Exit", "Leave the game"};

    const int sectionLabelY = 172;
    DrawText("Start Game", 80, sectionLabelY, 24, DARKGRAY);

    const int gameButtonHeight = ClampInt(screenHeight / 10, 56, 72);
    const int gameButtonWidth = ClampInt((screenWidth - 140) / 3, 96, 160);
    const int gameGap = ClampInt((screenWidth - 3 * gameButtonWidth - 80) / 2, 14, 24);
    const int gameGroupY = ClampInt(screenHeight / 2 - 40, 180, 230);
    const int gameGroupStartX = (screenWidth - (3 * gameButtonWidth + 2 * gameGap)) / 2;

    for (int i = 0; i < 3; ++i)
    {
        DrawMenuButton(gameGroupStartX + i * (gameButtonWidth + gameGap), gameGroupY, gameButtonWidth, gameButtonHeight, &gameEntries[i], i == selectedOption);
    }

    const int actionWidth = ClampInt(screenWidth - 140, 240, 420);
    const int actionHeight = ClampInt(screenHeight / 10, 54, 64);
    const int actionX = (screenWidth - actionWidth) / 2;
    const int settingsY = ClampInt(gameGroupY + gameButtonHeight + 32, 300, 360);
    const int exitY = ClampInt(settingsY + actionHeight + 18, 380, 440);

    DrawMenuButton(actionX, settingsY, actionWidth, actionHeight, &settingsEntry, selectedOption == MENU_ACTION_SETTINGS);
    DrawMenuButton(actionX, exitY, actionWidth, actionHeight, &exitEntry, selectedOption == MENU_ACTION_EXIT);

    DrawText("Use the arrow keys or mouse to navigate", (screenWidth - MeasureText("Use the arrow keys or mouse to navigate", 20)) / 2, screenHeight - 80, 20, GRAY);
    DrawText("Press Enter to confirm", (screenWidth - MeasureText("Press Enter to confirm", 18)) / 2, screenHeight - 48, 18, DARKGRAY);
}
