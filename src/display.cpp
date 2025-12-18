#include "display.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

const int EINK_BUSY = 7;
const int EINK_RST = 6;
const int EINK_DC = 5;
const int EINK_CS = 4;

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

void initDisplay(int currentVersion)
{
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  displayMessage("Starting up... \nv" + String(currentVersion));
}

void displayMessage(String message)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.print(message);
  } while (display.nextPage());
}