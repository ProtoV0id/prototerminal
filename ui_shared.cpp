#include "ui_shared.h"

#include "themes.h"
#include "wifi_manager.h"

TFT_eSPI tft = TFT_eSPI();

static void drawDefaultHeader(const char* title);
static void drawNightCityHeader(const char* title);
static void drawDefaultFooter();
static void drawNightCityFooter();

void drawHeader(const char* title) {

  if (isNightCity()) {

    drawNightCityHeader(title);

  } else {

    drawDefaultHeader(title);

  }

}

static void drawDefaultHeader(const char* title) {

  tft.setTextDatum(TL_DATUM);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.setTextSize(2);

  tft.drawString(title, 10, 7);

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString("SYS:", 228, 6);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString("READY", 258, 6);

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString("NET:", 228, 19);

  if (isWiFiConnected()) {

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString("ONLINE", 258, 19);

  } else {

    tft.setTextColor(

      activeTheme->warning,

      activeTheme->background

    );

    tft.drawString("OFFLINE", 258, 19);

  }

  tft.drawFastHLine(

    0,

    41,

    320,

    activeTheme->muted

  );

}

static void drawNightCityHeader(const char* title) {

  tft.setTextDatum(TL_DATUM);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.setTextSize(2);

  tft.drawString(title, 10, 5);

  tft.fillRect(

    10,

    29,

    78,

    3,

    activeTheme->secondary

  );

  tft.fillRect(

    91,

    29,

    18,

    3,

    activeTheme->warning

  );

  tft.setTextSize(1);

  tft.drawRect(

    216,

    4,

    98,

    14,

    activeTheme->secondary

  );

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString("SYS", 221, 7);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString("[READY]", 244, 7);

  tft.drawRect(

    216,

    21,

    98,

    14,

    activeTheme->warning

  );

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString("NET", 221, 24);

  if (isWiFiConnected()) {

    tft.setTextColor(

      activeTheme->secondary,

      activeTheme->background

    );

    tft.drawString("[ON]", 247, 24);

  } else {

    tft.setTextColor(

      activeTheme->warning,

      activeTheme->background

    );

    tft.drawString("[OFF]", 247, 24);

  }

  tft.fillRect(

    0,

    42,

    118,

    2,

    activeTheme->primary

  );

  tft.fillRect(

    122,

    42,

    84,

    2,

    activeTheme->secondary

  );

  tft.fillRect(

    210,

    42,

    44,

    2,

    activeTheme->warning

  );

  tft.fillRect(

    258,

    42,

    62,

    2,

    activeTheme->primary

  );

}

void drawFooter() {

  if (isNightCity()) {

    drawNightCityFooter();

  } else {

    drawDefaultFooter();

  }

}

static void drawDefaultFooter() {

  tft.drawFastHLine(

    0,

    218,

    320,

    activeTheme->muted

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString(

    "HOME",

    14,

    226

  );

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString(

    "BACK",

    142,

    226

  );

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString(

    "EXEC",

    268,

    226

  );

}

static void drawNightCityFooter() {

  tft.fillRect(

    0,

    217,

    72,

    2,

    activeTheme->primary

  );

  tft.fillRect(

    76,

    217,

    82,

    2,

    activeTheme->secondary

  );

  tft.fillRect(

    162,

    217,

    46,

    2,

    activeTheme->warning

  );

  tft.fillRect(

    212,

    217,

    108,

    2,

    activeTheme->primary

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString(

    "[HOME]",

    10,

    226

  );

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString(

    "[BACK]",

    136,

    226

  );

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString(

    "[EXEC]",

    250,

    226

  );

}
