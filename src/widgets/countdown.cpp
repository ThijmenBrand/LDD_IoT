#include "countdown.h"
#include <ArduinoJson.h>
#include "display.h"
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void drawCountdown(String jsonObjectPayload, GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display)
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