#include <FlashStorage.h>

#include "settings.h"
#include "themes.h"

const uint32_t SETTINGS_MAGIC = 0x50545632; // "PTV2"
const uint32_t OLD_SETTINGS_MAGIC = 0x50545631; // "PTV1"

struct PersistedSettings {
  uint32_t magic;
  uint8_t theme;

  char apiServerUrl[128];
  char apiKey[96];
};

FlashStorage(settingsStore, PersistedSettings);

static PersistedSettings savedSettings;

void initSettings() {
  savedSettings = settingsStore.read();

  bool isCurrent =
    savedSettings.magic == SETTINGS_MAGIC;

  bool isPrevious =
    savedSettings.magic == OLD_SETTINGS_MAGIC;

  // Preserve an existing valid theme.
  uint8_t theme = THEME_DEFAULT;

  if ((isCurrent || isPrevious) &&
      savedSettings.theme <= THEME_NIGHT_CITY) {
    theme = savedSettings.theme;
  }

  // Initialize or migrate configuration.
  if (!isCurrent) {
    savedSettings.magic = SETTINGS_MAGIC;
    savedSettings.theme = theme;

    memset(savedSettings.apiServerUrl, 0,
           sizeof(savedSettings.apiServerUrl));

    memset(savedSettings.apiKey, 0,
           sizeof(savedSettings.apiKey));

    settingsStore.write(savedSettings);
  }

  // Ensure character arrays are terminated.
  savedSettings.apiServerUrl[
    sizeof(savedSettings.apiServerUrl) - 1
  ] = '\0';

  savedSettings.apiKey[
    sizeof(savedSettings.apiKey) - 1
  ] = '\0';

  setTheme((ThemeType)savedSettings.theme);
}

void saveThemePreference(ThemeType theme) {
  setTheme(theme);

  savedSettings.magic = SETTINGS_MAGIC;
  savedSettings.theme = (uint8_t)theme;

  settingsStore.write(savedSettings);
}

ThemeType getSavedTheme() {
  return (ThemeType)savedSettings.theme;
}

// ============================================================
// PROTORECON API SETTINGS
// ============================================================

void saveApiConfiguration(
  const String &serverUrl,
  const String &apiKey
) {
  // Reject values that cannot fit in storage.
  if (serverUrl.length() >= sizeof(savedSettings.apiServerUrl) ||
      apiKey.length() >= sizeof(savedSettings.apiKey)) {
    return;
  }

  // Clear previous values.
  memset(savedSettings.apiServerUrl, 0,
         sizeof(savedSettings.apiServerUrl));

  memset(savedSettings.apiKey, 0,
         sizeof(savedSettings.apiKey));

  // Copy new settings.
  serverUrl.toCharArray(
    savedSettings.apiServerUrl,
    sizeof(savedSettings.apiServerUrl)
  );

  apiKey.toCharArray(
    savedSettings.apiKey,
    sizeof(savedSettings.apiKey)
  );

  // Persist configuration.
  settingsStore.write(savedSettings);
}


String getApiServerUrl() {
  return String(savedSettings.apiServerUrl);
}


String getApiKey() {
  return String(savedSettings.apiKey);
}


bool hasApiConfiguration() {
  return strlen(savedSettings.apiServerUrl) > 0 &&
         strlen(savedSettings.apiKey) > 0;
}
