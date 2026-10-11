#include "settings.h"
#include "wifi_manager.h"
#include "ui.h"
#include "serial_config.h"

void setup() {
  Serial.begin(115200);

  initSettings();
  initWiFi();
  initUI();
  initSerialConfig();

  showBootScreen(5000);
  drawCurrentScreen();
}

void loop() {
  updateUI();
  updateSerialConfig();
}
