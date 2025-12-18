#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "../lib/secrets.h"

const int MAX_HEIGHT = 130;
const int MAX_WIDHT = 300;

// Define pins for the E-Ink board
const int EINK_BUSY = 7;
const int EINK_RST = 6;
const int EINK_DC = 5;
const int EINK_CS = 4;

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

// API Settings
const char* server = "api.coindesk.com";
WiFiSSLClient client;


int counter = 0;

void setup() {
  Serial.begin(115200);

  // Initialize the e-paper
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // Show connecting message
  display.setFullWindow();
  display.firstPage();
do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.print("Hello world!");
  } while (display.nextPage());
}

void loop() {
}
