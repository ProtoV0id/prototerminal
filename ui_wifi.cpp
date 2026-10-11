#include "ui_wifi.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"
#include "wifi_manager.h"

static int scannedNetworkCount = 0;

static const char* wifiMenuItems[] = {
  "Connect",
  "Scan Networks",
  "Disconnect"
};

int getWiFiMenuCount() {
  return sizeof(wifiMenuItems) / sizeof(wifiMenuItems[0]);
}

void drawWiFiMenu() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "NET//CONFIG"

      : "WiFi"

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->secondary,

    activeTheme->background

  );

  if (isWiFiConnected()) {

    String ssidLine =

      "SSID: " +

      getWiFiSSID();

    String ipLine =

      "IP: " +

      getWiFiIP();

    if (ssidLine.length() > 24) {

      ssidLine =

        ssidLine.substring(

          0,

          24

        );

    }

    tft.drawString(

      ssidLine,

      18,

      49

    );

    tft.drawString(

      ipLine,

      172,

      49

    );

  } else {

    String configured;

    if (hasConfiguredWiFi()) {

      configured =

        String("SSID: ") +

        getConfiguredSSID();

    } else {

      configured =

        "SSID: NOT CONFIGURED";

    }

    if (configured.length() > 38) {

      configured =

        configured.substring(

          0,

          38

        );

    }

    tft.drawString(

      configured,

      18,

      49

    );

  }

  for (int i = 0; i < getWiFiMenuCount(); i++) {

    drawWiFiRow(i);

  }

  drawFooter();

}

void drawWiFiRow(int item) {

  int y =

    76 +

    item * 31;

  tft.fillRect(

    18,

    y - 2,

    280,

    25,

    activeTheme->background

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);

  if (item == wifiSelected) {

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString(

      ">",

      MENU_X,

      y

    );

  } else {

    tft.setTextColor(

      activeTheme->text,

      activeTheme->background

    );

  }

  tft.drawString(

    wifiMenuItems[item],

    MENU_X + 22,

    y

  );

}

void connectWiFiScreen() {

  if (!hasConfiguredWiFi()) {

    tft.fillScreen(activeTheme->background);

    drawHeader(

      isNightCity()

        ? "NET//ERROR"

        : "WiFi"

    );

    tft.setTextDatum(TL_DATUM);

    tft.setTextColor(

      activeTheme->warning,

      activeTheme->background

    );

    tft.setTextSize(2);

    tft.drawString(

      "No SSID configured.",

      20,

      82

    );

    tft.setTextSize(1);

    tft.setTextColor(

      activeTheme->text,

      activeTheme->background

    );

    tft.drawString(

      "Edit wifi_config.h",

      20,

      114

    );

    drawFooter();

    return;

  }

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "NET//UPLINK"

      : "WiFi Connect"

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString(

    "Connecting...",

    20,

    82

  );

  connectConfiguredWiFi();

  drawWiFiMenu();

}

void scanWiFiScreen() {

  currentScreen = SCREEN_WIFI_SCAN;

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "NET//SCAN"

      : "WiFi Scan"

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.setTextSize(2);

  tft.drawString(

    "Scanning...",

    25,

    92

  );

  scannedNetworkCount =

    scanWiFiNetworks();

  drawWiFiScanResults();

}

void drawWiFiScanResults() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "NET//SCAN"

      : "WiFi Scan"

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(1);

  if (scannedNetworkCount <= 0) {

    tft.setTextColor(

      activeTheme->warning,

      activeTheme->background

    );

    tft.drawString(

      "No networks found.",

      20,

      70

    );

  } else {

    int shown =

      min(

        scannedNetworkCount,

        7

      );

    for (int i = 0; i < shown; i++) {

      int y =

        55 +

        i * 21;

      tft.setTextColor(

        activeTheme->text,

        activeTheme->background

      );

      String line =

        getScannedSSID(i) +

        "  " +

        String(

          getScannedRSSI(i)

        ) +

        "dBm";

      if (line.length() > 39) {

        line =

          line.substring(

            0,

            39

          );

      }

      tft.drawString(

        line,

        16,

        y

      );

    }

  }

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString(

    "BACK returns",

    16,

    203

  );

  drawFooter();

}
