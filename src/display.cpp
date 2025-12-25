#include "display.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <ArduinoJson.h>

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

void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget)
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    display.setFont(&FreeMono9pt7b);
    display.setTextColor(GxEPD_BLACK);

    String batStr = String(batteryPercentage) + "%";
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
    return drawCalendar(data);
  }

  if (widget == "countdown")
  {
    return drawCountdown(data);
  }
  // Future widgets can be added here
}

void drawCalendar(String payload)
{
  JsonDocument doc;
  deserializeJson(doc, payload);

  JsonObject highlight = doc["highlight"];
  JsonArray upcoming = doc["upcoming"].as<JsonArray>();

  display.fillScreen(GxEPD_WHITE);

  // ============================================
  // LEFT SIDE: HIGHLIGHT (Inverted: Black BG, White Text)
  // ============================================
  int splitX = 150;

  // Draw the Box
  display.fillRect(0, 0, splitX, 128, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE); // Invert text color

  String type = highlight["type"].as<String>();

  if (type == "event")
  {
    display.setFont(&FreeMono9pt7b);
    display.setCursor(5, 20);
    display.print(highlight["time"].as<String>());

    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(5, 55);

    // Word Wrap Title
    String title = highlight["title"].as<String>();
    int spaceIndex = title.indexOf(' ');
    if (spaceIndex > 0 && title.length() > 8)
    {
      display.print(title.substring(0, spaceIndex));
      display.setCursor(5, 80);
      display.print(title.substring(spaceIndex + 1));
    }
    else
    {
      display.print(title);
    }
  }
  else
  {
    // --- DRAW "NO EVENTS" STATE ---
    // A nice big checkmark or smiley could go here
    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(5, 50);
    display.print("No Events");

    display.setFont(&FreeMono9pt7b);
    display.setCursor(5, 80);
    display.print("Today");
  }

  // ============================================
  // RIGHT SIDE: UPCOMING LIST (Normal: White BG, Black Text)
  // ============================================
  display.setTextColor(GxEPD_BLACK);
  int listX = splitX + 10; // Padding
  int y = 30;

  for (JsonVariant v : upcoming)
  {
    // Date/Time (Bold/Small)
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(listX, y);
    display.print(v["time"].as<String>());

    // Title (Normal/Small)
    y += 18; // Line spacing
    display.setFont(&FreeMono9pt7b);
    display.setCursor(listX, y);
    display.print(v["title"].as<String>());

    y += 25; // Gap to next item
  }
}

void drawCountdown(String jsonObjectPayload)
{
  JsonDocument doc;
  deserializeJson(doc, jsonObjectPayload);

  String daysLeft = doc["daysLeft"].as<String>();
  String dateString = doc["dateString"].as<String>();
  String label = doc["label"].as<String>();
  int progress = doc["progress"].as<int>();

  display.fillScreen(GxEPD_WHITE);

  display.setFont(&FreeMonoBold24pt7b);
  display.setTextColor(GxEPD_BLACK);

  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(daysLeft, 0, 0, &tbx, &tby, &tbw, &tbh);

  int x = (296 - tbw) / 2;
  display.setCursor(x, 50);
  display.print(daysLeft);

  display.setFont(&FreeMonoBold12pt7b);
  display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
  x = (296 - tbw) / 2;
  display.setCursor(x, 80);
  display.print(label);

  display.setFont(&FreeMono9pt7b);
  display.setCursor(10, 105);
  display.print(dateString);

  display.drawRect(10, 110, 276, 15, GxEPD_BLACK);

  int fillWidth = map(progress, 0, 100, 0, 272); // 276 width - 4px padding
  if (fillWidth > 0)
  {
    display.fillRect(12, 112, fillWidth, 11, GxEPD_BLACK);
  }
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

void getCenteredPosition(String text, int &x, int &y)
{
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);

  x = (display.width() - tbw) / 2;
  y = (display.height() - tbh) / 2 + tbh; // Adjust for baseline
}