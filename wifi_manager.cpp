#include "wifi_manager.h"
#include "wifi_config.h"

void initWiFi() {
  WiFi.mode(WIFI_STA);
}

bool connectConfiguredWiFi() {
  if (!hasConfiguredWiFi()) {
    return false;
  }

  WiFi.disconnect();
  delay(250);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long started = millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - started < 12000
  ) {
    delay(250);
  }

  return WiFi.status() == WL_CONNECTED;
}

void disconnectWiFi() {
  WiFi.disconnect();
}

int scanWiFiNetworks() {
  return WiFi.scanNetworks();
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

String getWiFiSSID() {
  if (!isWiFiConnected()) {
    return "";
  }

  return WiFi.SSID();
}

String getWiFiIP() {
  if (!isWiFiConnected()) {
    return "";
  }

  return WiFi.localIP().toString();
}

String getScannedSSID(int index) {
  return WiFi.SSID(index);
}

int getScannedRSSI(int index) {
  return WiFi.RSSI(index);
}

bool hasConfiguredWiFi() {
  return strlen(WIFI_SSID) > 0;
}

const char* getConfiguredSSID() {
  return WIFI_SSID;
}
