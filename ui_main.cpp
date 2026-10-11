#include "ui_main.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"

static const char* mainMenuItems[] = {
  "ProtoRecon",
  "Network",
  "Hardware",
  "Sensors",
  "Utilities",
  "Settings"
};

int getMainMenuCount() {
  return sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
}

void drawMainMenu() {

  tft.fillScreen(activeTheme->background);

  drawHeader("PROTO//TERM");

  for (int i = 0; i < getMainMenuCount(); i++) {

    drawMainRow(i);

  }

  drawFooter();

}

void drawMainRow(int item) {

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

  String label = mainMenuItems[item];

  if (isNightCity()) {

    label.toUpperCase();

  }

  if (item == mainSelected) {

    if (isNightCity()) {

      tft.fillRect(

        18,

        y,

        3,

        18,

        activeTheme->secondary

      );

      tft.fillRect(

        21,

        y + 14,

        8,

        3,

        activeTheme->warning

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
