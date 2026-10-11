#include "ui_about.h"
#include "ui_shared.h"
#include "themes.h"
#include "proto_logo_96.h"

void drawAbout() {
  tft.fillScreen(activeTheme->background);

  drawHeader(
    isNightCity() ? "ABOUT//SYS" : "About"
  );

  // --------------------------------------------------
  // Logo — top right in the main area
  // --------------------------------------------------
  tft.drawBitmap(
    212,          // right side
    52,           // just under header
    protoLogo96,
    96,
    96,
    activeTheme->primary
  );

  // --------------------------------------------------
  // Title block (left side)
  // --------------------------------------------------
  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);
  tft.setTextColor(activeTheme->primary, activeTheme->background);
  tft.drawString("ProtoTerminal", 18, 58);

  tft.setTextSize(1);
  tft.setTextColor(activeTheme->muted, activeTheme->background);
  tft.drawString("POCKET RECON TERMINAL", 18, 82);

  tft.setTextColor(activeTheme->text, activeTheme->background);
  tft.drawString("Built for Wio Terminal", 18, 100);

  // Accent line under title
  tft.fillRect(18, 118, 70, 2, activeTheme->secondary);

  // --------------------------------------------------
  // Info cards (bottom section)
  // --------------------------------------------------

  // Card 1 — Version
  tft.drawRect(14, 158, 140, 40, activeTheme->muted);
  tft.fillRect(14, 158, 4, 40, activeTheme->primary);

  tft.setTextSize(1);
  tft.setTextColor(activeTheme->muted, activeTheme->background);
  tft.drawString("VERSION", 28, 164);

  tft.setTextSize(2);
  tft.setTextColor(activeTheme->primary, activeTheme->background);
  tft.drawString("Beta 1", 28, 178);

  // Card 2 — Platform
  tft.drawRect(166, 158, 140, 40, activeTheme->muted);
  tft.fillRect(166, 158, 4, 40, activeTheme->secondary);

  tft.setTextSize(1);
  tft.setTextColor(activeTheme->muted, activeTheme->background);
  tft.drawString("NAME", 180, 164);

  tft.setTextSize(2);
  tft.setTextColor(activeTheme->text, activeTheme->background);
  tft.drawString("NightGlass", 180, 178);

  // --------------------------------------------------
  // Bottom tagline
  // --------------------------------------------------
  tft.setTextSize(1);
  tft.setTextColor(activeTheme->muted, activeTheme->background);
  tft.drawString("Settings > About", 18, 202);

  drawFooter();
}