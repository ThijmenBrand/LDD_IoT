#include "error.h"
#include "display.h"
#include "../images/images.h"
#include <Fonts/FreeMonoBold12pt7b.h>

void drawErrorScreen(String errorMessage, GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display)
{
  drawBitmap(nyuszi_logo, 30, 0, 235, 128); // Draw nyuszi logo at top right corner
}