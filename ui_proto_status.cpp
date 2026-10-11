#include "ui_proto_status.h"

#include "ui_shared.h"
#include "themes.h"
#include "wifi_manager.h"
#include "api_client.h"


void drawProtoReconStatus() {
  tft.fillScreen(activeTheme->background);

  drawHeader(
    isNightCity()
      ? "RECON//STATUS"
      : "ProtoRecon Status"
  );

  // --------------------------------------------------
  // WIFI OFFLINE
  // --------------------------------------------------

  if (!isWiFiConnected()) {
    tft.setTextDatum(TL_DATUM);

    tft.drawRect(
      14,
      58,
      292,
      94,
      activeTheme->warning
    );

    tft.fillRect(
      14,
      58,
      4,
      94,
      activeTheme->warning
    );

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "API STATUS",
      28,
      70
    );

    tft.setTextSize(2);

    tft.setTextColor(
      activeTheme->warning,
      activeTheme->background
    );

    tft.drawString(
      "OFFLINE",
      28,
      90
    );

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->text,
      activeTheme->background
    );

    tft.drawString(
      "WiFi is not connected.",
      28,
      122
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "Connect in Settings > WiFi",
      28,
      137
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC: REFRESH",
      235,
      203
    );

    drawFooter();
    return;
  }

  // --------------------------------------------------
  // REQUEST STATUS
  // --------------------------------------------------

  tft.setTextDatum(TL_DATUM);

  tft.setTextSize(2);

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    "Connecting...",
    20,
    78
  );

  ProtoReconStatus status;

  bool success =
    getProtoReconStatus(status);

  tft.fillRect(
    0,
    46,
    320,
    170,
    activeTheme->background
  );

  // --------------------------------------------------
  // API REQUEST FAILED
  // --------------------------------------------------

  if (!success) {
    tft.drawRect(
      14,
      58,
      292,
      94,
      activeTheme->warning
    );

    tft.fillRect(
      14,
      58,
      4,
      94,
      activeTheme->warning
    );

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "API STATUS",
      28,
      70
    );

    tft.setTextSize(2);

    tft.setTextColor(
      activeTheme->warning,
      activeTheme->background
    );

    tft.drawString(
      "OFFLINE",
      28,
      90
    );

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->text,
      activeTheme->background
    );

    tft.drawString(
      "Could not reach ProtoRecon.",
      28,
      122
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC: REFRESH",
      235,
      203
    );

    drawFooter();
    return;
  }

  // --------------------------------------------------
  // LAYOUT GEOMETRY
  // --------------------------------------------------

  const int leftX = 12;
  const int leftW = 196;

  const int rightX = 216;
  const int rightW = 92;

  const int topY = 53;
  const int boxH = 61;
  const int secondY = 121;

  // --------------------------------------------------
  // ACTIVE WORKSPACE BOX
  // --------------------------------------------------

  tft.drawRect(
    leftX,
    topY,
    leftW,
    boxH,
    activeTheme->muted
  );

  tft.fillRect(
    leftX,
    topY,
    4,
    boxH,
    activeTheme->secondary
  );

  if (isNightCity()) {
    tft.fillRect(
      leftX + 4,
      topY,
      54,
      2,
      activeTheme->warning
    );

    tft.fillRect(
      leftX + 62,
      topY,
      42,
      2,
      activeTheme->secondary
    );
  }

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    isNightCity()
      ? "ACTIVE//WORKSPACE"
      : "ACTIVE WORKSPACE",
    leftX + 12,
    topY + 9
  );

  String workspace = status.workspace;
  const int maxTextW = leftW - 24;

  tft.setTextSize(2);
  if (tft.textWidth(workspace) > maxTextW) {
    tft.setTextSize(1);
  }

  if (tft.textWidth(workspace) > maxTextW) {
    while (workspace.length() > 3 && tft.textWidth(workspace + "..") > maxTextW) {
      workspace.remove(workspace.length() - 1);
    }
    workspace += "..";
  }

  tft.setTextColor(
    activeTheme->secondary,
    activeTheme->background
  );

  tft.drawString(
    workspace,
    leftX + 12,
    topY + 29
  );

  // --------------------------------------------------
  // ACTIVE TARGET BOX
  // --------------------------------------------------

  tft.drawRect(
    leftX,
    secondY,
    leftW,
    boxH,
    activeTheme->muted
  );

  tft.fillRect(
    leftX,
    secondY,
    4,
    boxH,
    activeTheme->primary
  );

  if (isNightCity()) {
    tft.fillRect(
      leftX + 4,
      secondY,
      54,
      2,
      activeTheme->primary
    );

    tft.fillRect(
      leftX + 62,
      secondY,
      42,
      2,
      activeTheme->warning
    );

  tft.setTextSize(1);
  }

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    isNightCity()
      ? "ACTIVE//TARGET"
      : "ACTIVE TARGET",
    leftX + 12,
    secondY + 9
  );

  String target = status.target;

  tft.setTextSize(2);
  if (tft.textWidth(target) > maxTextW) {
    tft.setTextSize(1);
  }

  if (tft.textWidth(target) > maxTextW) {
    while (target.length() > 3 && tft.textWidth(target + "..") > maxTextW) {
      target.remove(target.length() - 1);
    }
    target += "..";
  }

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    target,
    leftX + 12,
    secondY + 29
  );

  // --------------------------------------------------
  // STATUS PANEL
  // --------------------------------------------------

  tft.drawRect(
    rightX,
    topY,
    rightW,
    129,
    activeTheme->muted
  );

  if (isNightCity()) {
    tft.fillRect(
      rightX,
      topY,
      rightW,
      2,
      activeTheme->primary
    );

    tft.fillRect(
      rightX,
      topY,
      3,
      129,
      activeTheme->warning
    );
  }

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    "API STATUS",
    rightX + 9,
    topY + 9
  );

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    "ONLINE",
    rightX + 9,
    topY + 24
  );

  tft.drawFastHLine(
    rightX + 8,
    topY + 42,
    rightW - 16,
    activeTheme->muted
  );

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    "WORKSPACES",
    rightX + 9,
    topY + 52
  );

  tft.setTextSize(2);

  tft.setTextColor(
    activeTheme->secondary,
    activeTheme->background
  );

  tft.drawString(
    String(status.workspaceCount),
    rightX + 9,
    topY + 66
  );

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    "TARGETS",
    rightX + 9,
    topY + 91
  );

  tft.setTextSize(2);

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    String(status.targetCount),
    rightX + 9,
    topY + 105
  );

  // --------------------------------------------------
  // REFRESH HINT
  // --------------------------------------------------

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->secondary,
    activeTheme->background
  );

  tft.drawString(
    isNightCity()
      ? "EXEC//REFRESH"
      : "EXEC: REFRESH",
    235,
    203
  );

  drawFooter();
}
