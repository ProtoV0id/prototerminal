#include "ui_settings.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"
#include "settings.h"

static const char* settingsMenuItems[] = {
  "Theme",
  "WiFi",
  "API",
  "About"
};

static const char* themeMenuItems[] = {
  "Default",
  "Night City"
};

int getSettingsMenuCount() {
  return sizeof(settingsMenuItems) / sizeof(settingsMenuItems[0]);
}

int getThemeMenuCount() {
  return sizeof(themeMenuItems) / sizeof(themeMenuItems[0]);
}

void drawSettingsMenu() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "SETTINGS//SYS"

      : "Settings"

  );

  for (int i = 0; i < getSettingsMenuCount(); i++) {

    drawSettingsRow(i);

  }

  drawFooter();

}

void drawSettingsRow(int item) {

  int y =

    MENU_Y +

    item * MENU_SPACING;

  tft.fillRect(

    18,

    y - 2,

    275,

    MENU_ROW_H + 4,

    activeTheme->background

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);

  String label = settingsMenuItems[item];

  if (isNightCity()) {

    label.toUpperCase();

  }

  if (item == settingsSelected) {

    if (isNightCity()) {

      tft.fillRect(

        18,

        y,

        3,

        18,

        activeTheme->secondary

      );

    }

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString(">", MENU_X, y);

  } else {

    tft.setTextColor(

      activeTheme->text,

      activeTheme->background

    );

  }

  tft.drawString(

    label,

    MENU_X + 22,

    y

  );

}

void drawThemeMenu() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "THEME//SELECT"

      : "Theme"

  );

  themeSelected =

    currentTheme == THEME_NIGHT_CITY ? 1 : 0;

  for (int i = 0; i < getThemeMenuCount(); i++) {

    drawThemeRow(i);

  }

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString(

    "Selection saves immediately.",

    28,

    144

  );

  drawFooter();

}

void drawThemeRow(int item) {

  int y =

    MENU_Y +

    item * MENU_SPACING;

  tft.fillRect(

    18,

    y - 2,

    285,

    MENU_ROW_H + 4,

    activeTheme->background

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);

  bool isActive =

    (

      item == 0 &&

      currentTheme == THEME_DEFAULT

    )

    ||

    (

      item == 1 &&

      currentTheme == THEME_NIGHT_CITY

    );

  if (item == themeSelected) {

    if (isNightCity()) {

      tft.fillRect(

        18,

        y,

        3,

        18,

        activeTheme->secondary

      );

    }

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString(">", MENU_X, y);

  } else {

    tft.setTextColor(

      activeTheme->text,

      activeTheme->background

    );

  }

  tft.drawString(

    themeMenuItems[item],

    MENU_X + 22,

    y

  );

  if (isActive) {

    tft.setTextSize(1);

    tft.setTextColor(

      activeTheme->secondary,

      activeTheme->background

    );

    tft.drawString(

      "[ACTIVE]",

      220,

      y + 5

    );

  }

}
