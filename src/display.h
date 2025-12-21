#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay();
void displayMessage(String message);
void drawLowBatteryWarning();
void showEmptyBatteryScreen();
void showUpdateScreen(int newVersion);

#endif // DISPLAY_H