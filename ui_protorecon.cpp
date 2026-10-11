#include "ui_protorecon.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"

static const char* protoReconMenuItems[] = {
  "Status",
  "Workspace",
  "Target"
};

int getProtoReconMenuCount() {
  return sizeof(protoReconMenuItems) / sizeof(protoReconMenuItems[0]);
}

void drawProtoReconMenu() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "PROTO//RECON"

      : "ProtoRecon"

  );

  for (int i = 0; i < getProtoReconMenuCount(); i++) {

    drawProtoReconRow(i);

  }

  drawFooter();

}

void drawProtoReconRow(int item) {

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

  String label = protoReconMenuItems[item];

  if (isNightCity()) {

    label.toUpperCase();

  }

  if (item == protoReconSelected) {

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
