#pragma once

#include <Arduino.h>
#include <rpcWiFi.h>

void initWiFi();

bool connectConfiguredWiFi();
void disconnectWiFi();

int scanWiFiNetworks();

bool isWiFiConnected();
String getWiFiSSID();
String getWiFiIP();

String getScannedSSID(int index);
int getScannedRSSI(int index);

bool hasConfiguredWiFi();
const char* getConfiguredSSID();
