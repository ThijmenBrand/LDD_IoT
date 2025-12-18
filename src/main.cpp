#include <Arduino.h>
#include "network.h"
#include "firmware.h"
#include "display.h"
#include "battery.h"

// DISPLAY SETTINGS
const int MAX_HEIGHT = 130;
const int MAX_WIDHT = 300;

void setup()
{
  Serial.begin(115200);

  // Initialize the e-paper
  initDisplay(CURRENT_VERSION);

  delay(1000);

  float volts = getBatteryVoltage();
  int percent = getBatteryPercentage();

  displayMessage("Battery:\n" + String(volts, 2) + " V\n" + String(percent) + " %");

  delay(5000);

  int WifiStatus = connectWifi();
  if (WifiStatus != WIFI_CONNECTED)
  {
    displayMessage("WiFi Connection\nFailed!");
    while (1)
      ;
  }

  int updateStatus = checkFirmwareUpdate();
  // Only check if an update is needed. If it fails or is up to date, continue normal operation.
  if (updateStatus == NEEDS_UPDATE)
  {
    displayMessage("Updating Firmware...");
    int result = updateFirmware();
    if (result == UPDATE_SUCCESSFUL)
    {
      displayMessage("Update Successful!\nRestarting...");
    }
    else
    {
      displayMessage("Update Failed!");
    }
  }

  displayMessage("Device is\nUp to Date!");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command = Serial.read();

    if (command == 'u')
    {
      Serial.println("Manual update check triggered.");
      checkFirmwareUpdate();
    }
  }
}