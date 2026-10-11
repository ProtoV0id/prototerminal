#include "ui_proto_target.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"
#include "wifi_manager.h"
#include "api_client.h"

int getSelectableTargetCount() {
  int count = 0;

  for (int i = 0; i < protoTargetList.count; i++) {
    if (
      protoTargetList.active.length() == 0 ||
      protoTargetList.items[i] != protoTargetList.active
    ) {
      count++;
    }
  }

  return count;
}

int getSelectableTargetIndex(int selectablePosition) {
  int position = 0;

  for (int i = 0; i < protoTargetList.count; i++) {
    if (
      protoTargetList.active.length() > 0 &&
      protoTargetList.items[i] == protoTargetList.active
    ) {
      continue;
    }

    if (position == selectablePosition) {
      return i;
    }

    position++;
  }

  return -1;
}

int getSelectedTargetPosition() {
  int position = 0;

  for (int i = 0; i < protoTargetList.count; i++) {
    if (
      protoTargetList.active.length() > 0 &&
      protoTargetList.items[i] == protoTargetList.active
    ) {
      continue;
    }

    if (i == protoTargetSelected) {
      return position;
    }

    position++;
  }

  return -1;
}

void loadProtoReconTargets() {
  protoTargetSelected = -1;
  protoTargetTop = 0;

  bool success =
    getProtoReconTargets(protoTargetList);

  if (!success) {
    protoTargetList.online = false;
    protoTargetList.count = 0;
    return;
  }

  if (getSelectableTargetCount() > 0) {
    protoTargetSelected =
      getSelectableTargetIndex(0);
  }
}

void drawProtoReconTarget() {
  tft.fillScreen(activeTheme->background);

  drawHeader(
    isNightCity()
      ? "RECON//TARGET"
      : "Target"
  );

  if (!isWiFiConnected()) {
    protoTargetList.online = false;
    protoTargetList.count = 0;

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

    drawFooter();
    return;
  }

  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(2);

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    "Loading...",
    20,
    82
  );

  loadProtoReconTargets();

  drawProtoReconTargetList();
}

void drawProtoReconTargetList() {
  tft.fillRect(
    0,
    46,
    320,
    170,
    activeTheme->background
  );

  tft.setTextDatum(TL_DATUM);

  // --------------------------------------------------
  // API OFFLINE
  // --------------------------------------------------

  if (!protoTargetList.online) {
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
      "Could not load targets.",
      28,
      122
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC: RETRY",
      235,
      203
    );

    drawFooter();
    return;
  }

  // --------------------------------------------------
  // NO TARGETS
  // --------------------------------------------------

  if (protoTargetList.count <= 0) {
    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "TARGET CONTROL",
      20,
      62
    );

    tft.setTextSize(2);

    tft.setTextColor(
      activeTheme->warning,
      activeTheme->background
    );

    tft.drawString(
      "No targets",
      20,
      84
    );

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->text,
      activeTheme->background
    );

    tft.drawString(
      "Add a target in ProtoRecon first.",
      20,
      116
    );

    drawFooter();
    return;
  }

  // --------------------------------------------------
  // ACTIVE TARGET SUMMARY
  // --------------------------------------------------

  if (isNightCity()) {
    tft.drawRect(
      14,
      51,
      292,
      43,
      activeTheme->secondary
    );

    tft.fillRect(
      14,
      51,
      5,
      43,
      activeTheme->primary
    );

    tft.fillRect(
      19,
      51,
      54,
      2,
      activeTheme->warning
    );

    tft.fillRect(
      77,
      51,
      42,
      2,
      activeTheme->secondary
    );

    tft.fillRect(
      123,
      51,
      32,
      2,
      activeTheme->primary
    );
  } else {
    tft.drawRect(
      14,
      51,
      292,
      43,
      activeTheme->muted
    );

    tft.fillRect(
      14,
      51,
      3,
      43,
      activeTheme->primary
    );
  }

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    isNightCity() ? "ACTIVE//TARGET" : "ACTIVE TARGET",
    24,
    56
  );

  bool hasActiveTarget =
    protoTargetList.active.length() > 0;

  tft.setTextColor(
    hasActiveTarget
      ? activeTheme->primary
      : activeTheme->warning,
    activeTheme->background
  );

  tft.drawString(
    hasActiveTarget ? "[ACTIVE]" : "[NONE]",
    248,
    56
  );

  String activeName =
    hasActiveTarget
      ? protoTargetList.active
      : "None";

  if (activeName.length() > 15) {
    activeName =
      activeName.substring(0, 13) + "..";
  }

  tft.setTextSize(3);

  tft.setTextColor(
    hasActiveTarget
      ? activeTheme->secondary
      : activeTheme->muted,
    activeTheme->background
  );

  tft.drawString(
    activeName,
    24,
    68
  );

  // --------------------------------------------------
  // SELECTABLE TARGETS
  // --------------------------------------------------

  int selectableCount =
    getSelectableTargetCount();

  if (selectableCount <= 0) {
    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "No other targets to select.",
      20,
      116
    );

    drawFooter();
    return;
  }

  const int visibleRows = 4;
  const int listY = 101;
  const int rowHeight = 24;

  int last = min(
    protoTargetTop + visibleRows,
    selectableCount
  );

  for (
    int position = protoTargetTop;
    position < last;
    position++
  ) {
    int i =
      getSelectableTargetIndex(position);

    if (i < 0) {
      continue;
    }

    int row =
      position - protoTargetTop;

    int y =
      listY + row * rowHeight;

    bool selected =
      i == protoTargetSelected;

    tft.fillRect(
      12,
      y - 1,
      296,
      22,
      activeTheme->background
    );

    if (selected) {
      if (isNightCity()) {
        tft.fillRect(
          14,
          y,
          4,
          20,
          activeTheme->primary
        );

        tft.fillRect(
          18,
          y + 17,
          8,
          3,
          activeTheme->warning
        );
      } else {
        tft.fillRect(
          14,
          y,
          3,
          20,
          activeTheme->primary
        );
      }

      tft.setTextColor(
        activeTheme->primary,
        activeTheme->background
      );
    } else {
      tft.setTextColor(
        activeTheme->text,
        activeTheme->background
      );
    }

    tft.setTextSize(2);

    if (selected) {
      tft.drawString(
        ">",
        22,
        y + 1
      );
    }

    String name =
      protoTargetList.items[i];

    if (name.length() > 20) {
      name =
        name.substring(0, 18) + "..";
    }

    tft.drawString(
      name,
      45,
      y + 1
    );
  }

  // --------------------------------------------------
  // LIST STATUS / ACTION HINT
  // --------------------------------------------------

  tft.setTextSize(1);

  tft.setTextColor(
    activeTheme->muted,
    activeTheme->background
  );

  int selectedPosition =
    getSelectedTargetPosition();

  if (isNightCity()) {
    tft.drawString(
      "[" + String(selectedPosition + 1) +
      "/" + String(selectableCount) + "]",
      16,
      201
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC//AIM",
      244,
      201
    );
  } else {
    tft.drawString(
      String(selectableCount) + " AVAILABLE",
      16,
      201
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC: AIM",
      244,
      201
    );
  }

  drawFooter();
}
