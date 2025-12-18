#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay(int currentVersion);
void displayMessage(String message);
void showUpdateScreen(int newVersion);

#endif // DISPLAY_H