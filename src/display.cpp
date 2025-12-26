#include "display.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <ArduinoJson.h>

#include "widgets/countdown.h"
#include "widgets/calendar.h"
#include "widgets/error.h"

const int EINK_BUSY = 7;
const int EINK_RST = 6;
const int EINK_DC = 5;
const int EINK_CS = 4;

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

void initDisplay()
{
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(GxEPD_BLACK);
}

void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    drawWidget(payload, widget);

    // --- 4. Draw Warnings ---
    if (isLowBattery)
    {
      drawLowBatteryWarning(batteryPercentage);
    }
  } while (display.nextPage());
}

void drawLowBatteryWarning(int batteryPercentage)
{
  drawFooter("WARNING: LOW BATTERY (" + String(batteryPercentage) + "%)", 0, 4);
}

void showEmptyBatteryScreen()
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    display.setFont(&FreeMonoBold12pt7b);
    display.setTextColor(GxEPD_BLACK);

    // Draw big centralized text
    display.setCursor(20, 100);
    display.print("Battery Empty!");

    display.setFont(&FreeMono9pt7b);
    display.setCursor(20, 140);
    display.print("Please recharge");
    display.setCursor(20, 160);
    display.print("device.");

  } while (display.nextPage());
}

void displayMessage(String message, int x, int y)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(message);
  } while (display.nextPage());
}

void displayMessage(String message, bool centered)
{
  int x = 10;
  int y = 30;
  if (centered)
  {
    getCenteredPosition(message, x, y);
  }
  displayMessage(message, x, y);
}

void drawWidget(String data, String widget)
{
  if (widget == "calendar")
  {
    return drawCalendar(data, display);
  }

  if (widget == "countdown")
  {
    return drawCountdown(data, display);
  }

  return drawErrorScreen("Error, but I love you nyuszi :)", display);
}

// The draw header function divides the header into four segments.
void drawHeader(String header, int startSegment, int segmentSpan)
{
  if (segmentSpan <= 0)
    return; // Invalid span
  if (startSegment < 0 || startSegment >= 4)
    return; // Invalid

  display.setFont(&FreeMono9pt7b);
  display.setTextColor(GxEPD_BLACK);

  int screenW = display.width();
  int segmentW = screenW / 4;

  int x = startSegment * segmentW + 10; // 10 pixels padding
  int y = 15;                           // Fixed Y position for header

  display.setCursor(x, y);
  display.print(header);
}

void drawFooter(String footer, int startSegment, int segmentSpan)
{
  if (segmentSpan <= 0)
    return; // Invalid span
  if (startSegment < 0 || startSegment >= 4)
    return; // Invalid
  if (startSegment + segmentSpan > 4)
    return; // Out of range

  display.setFont(&FreeMono9pt7b);
  display.setTextColor(GxEPD_BLACK);

  int screenW = display.width();
  int segmentW = screenW / 4;

  int x = startSegment * segmentW + 10; // 10 pixels padding
  int y = display.height() - 5;         // baseline Y position for footer

  // Clear area where the footer will be drawn so it overrides any existing content.
  int segX = startSegment * segmentW;
  int clearX = segX + 2;
  int clearW = segmentW * segmentSpan - 4;
  int footerH = 15; // height to clear (adjust if needed for font size)
  int clearY = y - footerH;
  display.fillRect(clearX, clearY, clearW, footerH, GxEPD_WHITE);

  display.setCursor(x, y);
  display.print(footer);
}

void drawBitmap(const unsigned char *bitmap, int x, int y, int w, int h)
{
  display.drawBitmap(x, y, bitmap, w, h, GxEPD_BLACK);
}

void getCenteredPosition(String text, int &x, int &y)
{
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);

  x = (display.width() - tbw) / 2;
  y = (display.height() - tbh) / 2 + tbh; // Adjust for baseline
}