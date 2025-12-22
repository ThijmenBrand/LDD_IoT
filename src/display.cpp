#include "display.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

const int EINK_BUSY = 7;
const int EINK_RST = 6;
const int EINK_DC = 5;
const int EINK_CS = 4;

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

void initDisplay()
{
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
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

    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(20, 140);
    display.print("Please recharge");
    display.setCursor(20, 160);
    display.print("device.");

  } while (display.nextPage());
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

void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);

    String batStr = String(batteryPercentage) + "%";
    drawHeader("Agenda:", 0, 2);
    drawHeader(batStr, 3, 1);

    drawWidget(payload, widget);

    // --- 4. Draw Warnings ---
    if (isLowBattery)
    {
      drawLowBatteryWarning(batteryPercentage);
    }
  } while (display.nextPage());
}

void drawWidget(String data, String widget)
{
  if (widget == "calendar")
  {
    return drawCalendarItems(data);
  }

  if (widget == "countdown")
  {
    return drawCountdown(data);
  }
  // Future widgets can be added here
}

void drawCalendarItems(String payload)
{
  int xPos = 10;
  int yPos = 40;    // Start position
  int padding = 15; // Space AFTER an item finishes

  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setTextWrap(true); // Ensure wrapping is on

  if (payload.length() == 0 || payload.startsWith("Error"))
  {
    display.setCursor(xPos, yPos);
    display.print("No upcoming events");
    return;
  }

  int lastIndex = 0;
  int separatorIndex = payload.indexOf('|');

  while (true)
  {
    String eventItem;

    if (separatorIndex == -1)
    {
      eventItem = payload.substring(lastIndex);
    }
    else
    {
      eventItem = payload.substring(lastIndex, separatorIndex);
    }

    display.setCursor(xPos, yPos);
    display.print(eventItem);
    yPos = display.getCursorY() + padding;

    if (yPos > display.height())
      break;

    if (separatorIndex == -1)
      break;

    lastIndex = separatorIndex + 1;
    separatorIndex = payload.indexOf('|', lastIndex);
  }
}

void drawCountdown(String payload)
{
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);

  display.setCursor(20, 40);
  display.print(payload);
}

// The draw header function divides the header into four segments.
void drawHeader(String header, int startSegment, int segmentSpan)
{
  if (segmentSpan <= 0)
    return; // Invalid span
  if (startSegment < 0 || startSegment >= 4)
    return; // Invalid

  display.setFont(&FreeMonoBold9pt7b);
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

  display.setFont(&FreeMonoBold9pt7b);
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