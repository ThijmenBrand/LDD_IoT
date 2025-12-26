#include "calendar.h"
#include <ArduinoJson.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void drawCalendar(String payload, GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display)
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