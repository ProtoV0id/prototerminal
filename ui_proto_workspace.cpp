#include "ui_proto_workspace.h"

#include "ui_shared.h"
#include "ui_state.h"
#include "themes.h"
#include "wifi_manager.h"
#include "api_client.h"

int getSelectableWorkspaceCount() {

  int count = 0;

  for (int i = 0; i < protoWorkspaceList.count; i++) {

    if (
      protoWorkspaceList.items[i] !=
      protoWorkspaceList.active
    ) {
      count++;
    }
  }

  return count;
}

int getSelectableWorkspaceIndex(int selectablePosition) {

  int position = 0;

  for (int i = 0; i < protoWorkspaceList.count; i++) {

    if (
      protoWorkspaceList.items[i] ==
      protoWorkspaceList.active
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

int getSelectedWorkspacePosition() {

  int position = 0;

  for (int i = 0; i < protoWorkspaceList.count; i++) {

    if (
      protoWorkspaceList.items[i] ==
      protoWorkspaceList.active
    ) {
      continue;
    }

    if (i == protoWorkspaceSelected) {
      return position;
    }

    position++;
  }

  return -1;
}

void loadProtoReconWorkspaces() {

  protoWorkspaceSelected = -1;
  protoWorkspaceTop = 0;

  bool success =
    getProtoReconWorkspaces(protoWorkspaceList);

  if (!success) {
    protoWorkspaceList.online = false;
    protoWorkspaceList.count = 0;
    return;
  }

  // The active workspace is displayed in the summary panel only.
  // Start the selector on the first different workspace.
  if (getSelectableWorkspaceCount() > 0) {
    protoWorkspaceSelected =
      getSelectableWorkspaceIndex(0);
  }
}

void drawProtoReconWorkspace() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "RECON//WORKSPACE"

      : "Workspace"

  );

  if (!isWiFiConnected()) {
    protoWorkspaceList.online = false;
    protoWorkspaceList.count = 0;

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

  loadProtoReconWorkspaces();

  drawProtoReconWorkspaceList();
}

void drawProtoReconWorkspaceList() {

  // Clear the content area while preserving the header/footer.
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

  if (!protoWorkspaceList.online) {
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
      "Could not load workspaces.",
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
  // NO WORKSPACES
  // --------------------------------------------------

  if (protoWorkspaceList.count <= 0) {

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "WORKSPACE CONTROL",
      20,
      62
    );

    tft.setTextSize(2);

    tft.setTextColor(
      activeTheme->warning,
      activeTheme->background
    );

    tft.drawString(
      "No workspaces",
      20,
      84
    );

    drawFooter();
    return;
  }

  // --------------------------------------------------
  // ACTIVE WORKSPACE SUMMARY
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
    isNightCity() ? "ACTIVE//WORKSPACE" : "ACTIVE WORKSPACE",
    24,
    56
  );

  tft.setTextColor(
    activeTheme->primary,
    activeTheme->background
  );

  tft.drawString(
    "[ACTIVE]",
    248,
    56
  );

  String activeName = protoWorkspaceList.active;

  if (activeName.length() > 15) {
    activeName = activeName.substring(0, 13) + "..";
  }

  // Active workspace is intentionally the largest text on this page.
  tft.setTextSize(3);

  tft.setTextColor(
    activeTheme->secondary,
    activeTheme->background
  );

  tft.drawString(
    activeName,
    24,
    68
  );

  // --------------------------------------------------
  // SELECTABLE WORKSPACES
  // --------------------------------------------------

  int selectableCount = getSelectableWorkspaceCount();

  if (selectableCount <= 0) {

    tft.setTextSize(1);

    tft.setTextColor(
      activeTheme->muted,
      activeTheme->background
    );

    tft.drawString(
      "No other workspaces to select.",
      20,
      116
    );

    tft.drawString(
      "Create another workspace in ProtoRecon.",
      20,
      136
    );

    drawFooter();
    return;
  }

  const int visibleRows = 4;
  const int listY = 101;
  const int rowHeight = 24;

  int last = min(
    protoWorkspaceTop + visibleRows,
    selectableCount
  );

  for (
    int position = protoWorkspaceTop;
    position < last;
    position++
  ) {

    int i =
      getSelectableWorkspaceIndex(position);

    if (i < 0) {
      continue;
    }

    int row =
      position - protoWorkspaceTop;

    int y =
      listY + row * rowHeight;

    bool selected =
      i == protoWorkspaceSelected;

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

    // Larger than the old list, but still smaller than activeName.
    tft.setTextSize(2);

    if (selected) {

      tft.drawString(
        ">",
        22,
        y + 1
      );
    }

    String name =
      protoWorkspaceList.items[i];

    if (name.length() > 20) {
      name = name.substring(0, 18) + "..";
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
    getSelectedWorkspacePosition();

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
      "EXEC//USE",
      244,
      201
    );

  } else {

    String countText =
      String(selectableCount) +
      " AVAILABLE";

    tft.drawString(
      countText,
      16,
      201
    );

    tft.setTextColor(
      activeTheme->secondary,
      activeTheme->background
    );

    tft.drawString(
      "EXEC: USE",
      244,
      201
    );
  }

  drawFooter();
}
