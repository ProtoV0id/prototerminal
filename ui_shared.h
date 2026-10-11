#pragma once

#include <TFT_eSPI.h>

extern TFT_eSPI tft;

constexpr int MENU_X = 28;
constexpr int MENU_Y = 60;
constexpr int MENU_SPACING = 25;
constexpr int MENU_ROW_H = 21;

void drawHeader(const char* title);
void drawFooter();
