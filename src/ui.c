#include "ui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void DrawScreenBox(const char *title, const char *instructions)
{
    const int boxX = 100;
    const int boxY = 100;
    const int boxWidth = 600;
    const int boxHeight = 400;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.55f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, GRAY);

    const int titleWidth = MeasureText(title, 40);
    DrawText(title, boxX + (boxWidth - titleWidth) / 2, boxY + 60, 40, DARKGRAY);

    const int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, boxX + (boxWidth - instructionsWidth) / 2, boxY + 200, 20, GRAY);
}

void ApplyScrabbleTheme(int baseFontSize)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, baseFontSize-2);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 16); 
    
    GuiSetStyle(STATUSBAR, TEXT_PADDING, 14);

    GuiSetStyle(BUTTON, TEXT_PADDING, 12);
    GuiSetStyle(BUTTON, COMBO_BUTTON_SPACING, 24); 

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x182026FF);
    GuiSetStyle(DEFAULT, LINE_COLOR, 0xC4B589FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xAACF9BFF);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0x8F8FA0FF);

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x2ECA7115);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x2ECA71AA);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0x2ECA71FF);

    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x2ECA7130);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0x8FF2B8FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x242C34FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x364452FF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0x8F8FA0FF);

    GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0x2ECA71FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_PRESSED, 0x8FF2B8FF);

    GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, 0x182026FF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0xC4B589FF);
}