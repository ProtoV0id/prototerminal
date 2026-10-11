#pragma once

#include "app_types.h"

extern Theme defaultTheme;
extern Theme nightCityTheme;

extern ThemeType currentTheme;
extern Theme* activeTheme;

void setTheme(ThemeType theme);
bool isNightCity();
