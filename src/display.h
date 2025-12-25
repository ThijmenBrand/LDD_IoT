#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay();
void displayMessage(String message, int x = 10, int y = 30);
void displayMessage(String message, bool centered);
void drawLowBatteryWarning(int batteryPercentage);
void showEmptyBatteryScreen();
void drawScreen(String payload, int batteryPercentage, bool isLowBattery, String widget);
void drawCalendar(String payload);
void drawCountdown(String payload);
void drawErrorScreen(String errorMessage);
void drawWidget(String data, String widget);
void drawHeader(String header, int startSegment, int segmentSpan);
void drawFooter(String footer, int startSegment, int segmentSpan);
void getCenteredPosition(String text, int &x, int &y);

#endif // DISPLAY_H