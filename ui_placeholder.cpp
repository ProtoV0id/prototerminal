#include "ui_placeholder.h"

#include "ui_shared.h"
#include "themes.h"

void drawPlaceholder() {

  tft.fillScreen(activeTheme->background);

  drawHeader(

    isNightCity()

      ? "MODULE//LOCKED"

      : "Module"

  );

  tft.setTextDatum(TL_DATUM);

  tft.setTextColor(

    activeTheme->text,

    activeTheme->background

  );

  tft.setTextSize(2);

  tft.drawString(

    "Module coming soon.",

    30,

    95

  );

  tft.setTextSize(1);

  tft.setTextColor(

    activeTheme->muted,

    activeTheme->background

  );

  tft.drawString(

    "UI shell is working.",

    30,

    124

  );

  drawFooter();

}
