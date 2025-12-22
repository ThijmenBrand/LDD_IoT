#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay();
void displayMessage(String message);
void drawLowBatteryWarning(int batteryPercentage);
void showEmptyBatteryScreen();
void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget);
void drawCalendarItems(String payload);
void drawCountdown(String payload);
void drawErrorScreen(String errorMessage);
void drawWidget(String data, String widget);
void drawHeader(String header, int startSegment, int segmentSpan);
void drawFooter(String footer, int startSegment, int segmentSpan);

#endif // DISPLAY_H