#include "ui.h"

void DrawScreenBox(const char* title, const char* instructions)
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
