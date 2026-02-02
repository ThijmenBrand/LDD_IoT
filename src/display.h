#pragma once

#include <Arduino.h>

void initDisplay();
void displayMessage(String message, int x = 10, int y = 30);
void displayMessage(String message, bool centered);
void drawLowBatteryWarning(int batteryPercentage);
void showEmptyBatteryScreen();
void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget);
void drawWidget(String data, String widget);
void drawHeader(String header, int startSegment, int segmentSpan);
void drawFooter(String footer, int startSegment, int segmentSpan);
void drawError(String errorMessage);
void drawBitmap(const unsigned char *bitmap, int x, int y, int w, int h);
void getCenteredPosition(String text, int &x, int &y);