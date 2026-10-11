#include <TFT_eSPI.h>
#include "themes.h"

Theme defaultTheme = {
  TFT_BLACK,
  TFT_GREEN,
  TFT_CYAN,
  TFT_WHITE,
  TFT_DARKGREY,
  TFT_MAGENTA
};

Theme nightCityTheme = {
  TFT_BLACK,
  TFT_YELLOW,
  TFT_CYAN,
  TFT_WHITE,
  TFT_DARKGREY,
  TFT_MAGENTA
};

ThemeType currentTheme = THEME_DEFAULT;
Theme* activeTheme = &defaultTheme;

void setTheme(ThemeType theme) {
  currentTheme = theme;

  if (theme == THEME_NIGHT_CITY) {
    activeTheme = &nightCityTheme;
  } else {
    activeTheme = &defaultTheme;
  }
}

bool isNightCity() {
  return currentTheme == THEME_NIGHT_CITY;
}
