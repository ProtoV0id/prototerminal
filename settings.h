#pragma once

#include "app_types.h"

void initSettings();
void saveThemePreference(ThemeType theme);
ThemeType getSavedTheme();

// ============================================================
// PROTORECON API CONFIGURATION
// ============================================================

// Save the FastAPI server address and authentication key.
void saveApiConfiguration(
  const String &serverUrl,
  const String &apiKey
);

// Retrieve saved configuration.
String getApiServerUrl();
String getApiKey();

// Check whether the device has API settings.
bool hasApiConfiguration();