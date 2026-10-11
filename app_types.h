#pragma once
#include <Arduino.h>

enum Screen {
  SCREEN_MAIN,
  SCREEN_PROTORECON,
  SCREEN_PROTO_STATUS,
  SCREEN_PROTO_WORKSPACE,
  SCREEN_PROTO_TARGET,
  SCREEN_SETTINGS,
  SCREEN_THEME,
  SCREEN_WIFI,
  SCREEN_WIFI_SCAN,
  SCREEN_ABOUT,
  SCREEN_PLACEHOLDER
};

enum ThemeType {
  THEME_DEFAULT,
  THEME_NIGHT_CITY
};

struct Theme {
  uint16_t background;
  uint16_t primary;
  uint16_t secondary;
  uint16_t text;
  uint16_t muted;
  uint16_t warning;
};
