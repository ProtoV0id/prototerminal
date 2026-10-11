#include <Arduino.h>

#include "ui.h"
#include "ui_state.h"
#include "ui_shared.h"

#include "app_types.h"
#include "themes.h"
#include "settings.h"
#include "wifi_manager.h"
#include "api_client.h"
#include "proto_logo_96.h"

#include "ui_main.h"
#include "ui_protorecon.h"
#include "ui_proto_status.h"
#include "ui_proto_workspace.h"
#include "ui_proto_target.h"
#include "ui_settings.h"
#include "ui_wifi.h"
#include "ui_about.h"
#include "ui_placeholder.h"

static unsigned long lastInputTime = 0;
static const unsigned long INPUT_DELAY = 180;

static void readControls();
static void moveSelection(int direction);
static void executeSelection();
static void goBack();

void initUI() {

  tft.begin();

  tft.setRotation(3);

  pinMode(WIO_KEY_A, INPUT_PULLUP);

  pinMode(WIO_KEY_B, INPUT_PULLUP);

  pinMode(WIO_KEY_C, INPUT_PULLUP);

  pinMode(WIO_5S_UP, INPUT_PULLUP);

  pinMode(WIO_5S_DOWN, INPUT_PULLUP);

  pinMode(WIO_5S_LEFT, INPUT_PULLUP);

  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);

  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  themeSelected =

    currentTheme == THEME_NIGHT_CITY ? 1 : 0;

}

void updateUI() {

  readControls();

}

void showBootScreen(unsigned long durationMs) {

  tft.fillScreen(activeTheme->background);

  tft.drawBitmap(

    112,

    16,

    protoLogo96,

    96,

    96,

    activeTheme->primary

  );

  tft.setTextDatum(MC_DATUM);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.setTextSize(2);

  tft.drawString("PROTO//TERM", 160, 137);

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.drawString("INITIALIZING", 160, 162);

  tft.setTextDatum(TL_DATUM);

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString("UI CORE", 78, 187);

  tft.setTextColor(

    activeTheme->primary,

    activeTheme->background

  );

  tft.drawString("OK", 226, 187);

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString("NETWORK", 78, 201);

  if (isWiFiConnected()) {

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString("OK", 226, 201);

  } else {

    tft.setTextColor(

      activeTheme->muted,

      activeTheme->background

    );

    tft.drawString("--", 226, 201);

  }

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString("API", 78, 215);

  tft.drawString("--", 226, 215);

  delay(durationMs);

}

void drawCurrentScreen() {
  if (currentScreen == SCREEN_MAIN) {
    drawMainMenu();
  }

  else if (currentScreen == SCREEN_PROTORECON) {
    drawProtoReconMenu();
  }

  else if (currentScreen == SCREEN_PROTO_STATUS) {
    drawProtoReconStatus();
  }

  else if (currentScreen == SCREEN_PROTO_WORKSPACE) {
    drawProtoReconWorkspace();
  }

  else if (currentScreen == SCREEN_PROTO_TARGET) {
    drawProtoReconTarget();
  }

  else if (currentScreen == SCREEN_SETTINGS) {
    drawSettingsMenu();
  }

  else if (currentScreen == SCREEN_THEME) {
    drawThemeMenu();
  }

  else if (currentScreen == SCREEN_WIFI) {
    drawWiFiMenu();
  }

  else if (currentScreen == SCREEN_WIFI_SCAN) {
    drawWiFiScanResults();
  }
  
  else if (currentScreen == SCREEN_ABOUT) {
    drawAbout();
  }

  else {
    drawPlaceholder();
  }
}

static void readControls() {

  if (millis() - lastInputTime < INPUT_DELAY) {

    return;

  }

  if (digitalRead(WIO_5S_UP) == LOW) {

    lastInputTime = millis();

    moveSelection(-1);

    return;

  }

  if (digitalRead(WIO_5S_DOWN) == LOW) {

    lastInputTime = millis();

    moveSelection(1);

    return;

  }

  // EXEC:

  // center 5-way or physical RIGHT top button

  if (

    digitalRead(WIO_5S_PRESS) == LOW ||

    digitalRead(WIO_KEY_A) == LOW

  ) {

    lastInputTime = millis();

    executeSelection();

    return;

  }

  // BACK:

  // left 5-way or physical MIDDLE top button

  if (

    digitalRead(WIO_5S_LEFT) == LOW ||

    digitalRead(WIO_KEY_B) == LOW

  ) {

    lastInputTime = millis();

    goBack();

    return;

  }

  // HOME:

  // physical LEFT top button

  if (digitalRead(WIO_KEY_C) == LOW) {

    lastInputTime = millis();

    currentScreen = SCREEN_MAIN;

    drawCurrentScreen();

    return;

  }

}

static void moveSelection(int direction) {
  if (currentScreen == SCREEN_MAIN) {
    int oldItem = mainSelected;
    mainSelected += direction;

    if (mainSelected < 0) {
      mainSelected = getMainMenuCount() - 1;
    }

    if (mainSelected >= getMainMenuCount()) {
      mainSelected = 0;
    }

    drawMainRow(oldItem);
    drawMainRow(mainSelected);
  }

  else if (currentScreen == SCREEN_PROTORECON) {
    int oldItem = protoReconSelected;
    protoReconSelected += direction;

    if (protoReconSelected < 0) {
      protoReconSelected = getProtoReconMenuCount() - 1;
    }

    if (protoReconSelected >= getProtoReconMenuCount()) {
      protoReconSelected = 0;
    }

    drawProtoReconRow(oldItem);
    drawProtoReconRow(protoReconSelected);
  }

  else if (currentScreen == SCREEN_PROTO_WORKSPACE) {
    int selectableCount = getSelectableWorkspaceCount();

    if (selectableCount <= 0) {
      return;
    }

    int selectedPosition = getSelectedWorkspacePosition();

    if (selectedPosition < 0) {
      selectedPosition = 0;
    }

    selectedPosition += direction;

    if (selectedPosition < 0) {
      selectedPosition = selectableCount - 1;
    }

    if (selectedPosition >= selectableCount) {
      selectedPosition = 0;
    }

    protoWorkspaceSelected = getSelectableWorkspaceIndex(selectedPosition);

    const int visibleRows = 4;

    if (selectedPosition < protoWorkspaceTop) {
      protoWorkspaceTop = selectedPosition;
    }

    if (selectedPosition >= protoWorkspaceTop + visibleRows) {
      protoWorkspaceTop = selectedPosition - visibleRows + 1;
    }

    drawProtoReconWorkspaceList();
  }

  else if (currentScreen == SCREEN_PROTO_TARGET) {
    int selectableCount = getSelectableTargetCount();

    if (selectableCount <= 0) {
      return;
    }

    int selectedPosition = getSelectedTargetPosition();

    if (selectedPosition < 0) {
      selectedPosition = 0;
    }

    selectedPosition += direction;

    if (selectedPosition < 0) {
      selectedPosition = selectableCount - 1;
    }

    if (selectedPosition >= selectableCount) {
      selectedPosition = 0;
    }

    protoTargetSelected =
      getSelectableTargetIndex(selectedPosition);

    const int visibleRows = 4;

    if (selectedPosition < protoTargetTop) {
      protoTargetTop = selectedPosition;
    }

    if (selectedPosition >= protoTargetTop + visibleRows) {
      protoTargetTop =
        selectedPosition - visibleRows + 1;
    }

    drawProtoReconTargetList();
  }

  else if (currentScreen == SCREEN_SETTINGS) {
    int oldItem = settingsSelected;
    settingsSelected += direction;

    if (settingsSelected < 0) {
      settingsSelected = getSettingsMenuCount() - 1;
    }

    if (settingsSelected >= getSettingsMenuCount()) {
      settingsSelected = 0;
    }

    drawSettingsRow(oldItem);
    drawSettingsRow(settingsSelected);
  }

  else if (currentScreen == SCREEN_THEME) {
    int oldItem = themeSelected;
    themeSelected += direction;

    if (themeSelected < 0) {
      themeSelected = getThemeMenuCount() - 1;
    }

    if (themeSelected >= getThemeMenuCount()) {
      themeSelected = 0;
    }

    drawThemeRow(oldItem);
    drawThemeRow(themeSelected);
  }

  else if (currentScreen == SCREEN_WIFI) {
    int oldItem = wifiSelected;
    wifiSelected += direction;

    if (wifiSelected < 0) {
      wifiSelected = getWiFiMenuCount() - 1;
    }

    if (wifiSelected >= getWiFiMenuCount()) {
      wifiSelected = 0;
    }

    drawWiFiRow(oldItem);
    drawWiFiRow(wifiSelected);
  }
}

static void executeSelection() {

  if (currentScreen == SCREEN_MAIN) {

    // ProtoRecon

    if (mainSelected == 0) {

      currentScreen = SCREEN_PROTORECON;

    }

    // Settings

    else if (mainSelected == 5) {

      currentScreen = SCREEN_SETTINGS;

    }

    else {

      currentScreen = SCREEN_PLACEHOLDER;

    }

    drawCurrentScreen();

    return;

  }

  if (currentScreen == SCREEN_PROTORECON) {

    if (protoReconSelected == 0) {

      currentScreen = SCREEN_PROTO_STATUS;

    }

    else if (protoReconSelected == 1) {

      currentScreen = SCREEN_PROTO_WORKSPACE;

    }

    else if (protoReconSelected == 2) {

      currentScreen = SCREEN_PROTO_TARGET;

    }

    drawCurrentScreen();

    return;

  }

  if (currentScreen == SCREEN_PROTO_STATUS) {

    // EXEC refreshes the API status screen.

    drawProtoReconStatus();

    return;

  }

  if (currentScreen == SCREEN_PROTO_WORKSPACE) {

    if (getSelectableWorkspaceCount() <= 0) {

      drawProtoReconWorkspace();

      return;

    }

    String selectedWorkspace =

      protoWorkspaceList.items[protoWorkspaceSelected];

    tft.fillRect(

      0,

      46,

      320,

      170,

      activeTheme->background

    );

    tft.setTextDatum(TL_DATUM);

    tft.setTextSize(2);

    tft.setTextColor(

      activeTheme->primary,

      activeTheme->background

    );

    tft.drawString(

      "Switching...",

      20,

      88

    );

    bool success =

      setProtoReconWorkspace(selectedWorkspace);

    if (success) {

      loadProtoReconWorkspaces();

    }

    drawProtoReconWorkspaceList();

    return;

  }

  if (currentScreen == SCREEN_PROTO_TARGET) {

    if (getSelectableTargetCount() <= 0) {

      drawProtoReconTarget();

      return;

    }

    String selectedTarget =
      protoTargetList.items[protoTargetSelected];

    tft.fillRect(
      0,
      46,
      320,
      170,
      activeTheme->background
    );

    tft.setTextDatum(TL_DATUM);
    tft.setTextSize(2);

    tft.setTextColor(
      activeTheme->primary,
      activeTheme->background
    );

    tft.drawString(
      "Aiming...",
      20,
      88
    );

    bool success =
      setProtoReconTarget(selectedTarget);

    if (success) {
      loadProtoReconTargets();
    }

    drawProtoReconTargetList();
    return;
  }

  if (currentScreen == SCREEN_SETTINGS) {

    if (settingsSelected == 0) {

      currentScreen = SCREEN_THEME;

    }

    else if (settingsSelected == 1) {

      currentScreen = SCREEN_WIFI;

    }

    //else if (settingsSelected ==2) { <API SCREEN NOT IMPLEMENTED YET>
      //currentScreen == SCREEN_API;
    //}

    else if (settingsSelected == 3) {
      currentScreen = SCREEN_ABOUT;
    }

    else {

      currentScreen = SCREEN_PLACEHOLDER;

    }

    drawCurrentScreen();

    return;

  }

  if (currentScreen == SCREEN_THEME) {

    if (themeSelected == 0) {

      saveThemePreference(THEME_DEFAULT);

    } else {

      saveThemePreference(THEME_NIGHT_CITY);

    }

    drawCurrentScreen();

    return;

  }

  if (currentScreen == SCREEN_WIFI) {

    if (wifiSelected == 0) {

      connectWiFiScreen();

    }

    else if (wifiSelected == 1) {

      scanWiFiScreen();

    }

    else if (wifiSelected == 2) {

      disconnectWiFi();

      drawWiFiMenu();

    }

    return;

  }

}

static void goBack() {

  if (

    currentScreen == SCREEN_PROTO_STATUS ||

    currentScreen == SCREEN_PROTO_WORKSPACE ||

    currentScreen == SCREEN_PROTO_TARGET

  ) {

    currentScreen = SCREEN_PROTORECON;

  }

  else if (currentScreen == SCREEN_PROTORECON) {

    currentScreen = SCREEN_MAIN;

  }

  else if (

    currentScreen == SCREEN_THEME ||

    currentScreen == SCREEN_WIFI

  ) {

    currentScreen = SCREEN_SETTINGS;

  }

  else if (currentScreen == SCREEN_WIFI_SCAN) {

    currentScreen = SCREEN_WIFI;

  }

  else if (currentScreen == SCREEN_ABOUT) {
    currentScreen = SCREEN_SETTINGS;
  }

  else if (

    currentScreen == SCREEN_SETTINGS ||

    currentScreen == SCREEN_PLACEHOLDER

  ) {

    currentScreen = SCREEN_MAIN;

  }


  else {

    currentScreen = SCREEN_MAIN;

  }

  drawCurrentScreen();

}
