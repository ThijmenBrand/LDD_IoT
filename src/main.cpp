#include <Arduino.h>
#include "network.h"
#include "firmware.h"
#include "display.h"
#include "battery.h"
#include "api.h"

// DISPLAY SETTINGS
const int MAX_HEIGHT = 130;
const int MAX_WIDHT = 300;

const String DEVICE_ID = "DEVICE_001";

void setup()
{
  Serial.begin(115200);

  // Initialize the e-paper
  initDisplay(CURRENT_VERSION);
  delay(1000);

  int WifiStatus = connectWifi();
  if (WifiStatus != WIFI_CONNECTED)
  {
    displayMessage("WiFi Connection\nFailed!");
    while (1)
      ;
  }

  int updateStatus = checkFirmwareUpdate();
  Serial.println("Firmware check result: " + String(updateStatus));
  // Only check if an update is needed. If it fails or is up to date, continue normal operation.
  if (updateStatus == NEEDS_UPDATE)
  {
    displayMessage("Updating Firmware...");
    delay(1000);
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

  // Fetch content from API
  String screenContent = fetchScreenContent(DEVICE_ID);
  if (screenContent == "")
  {
    displayMessage("Failed to fetch\nscreen content!");
    while (1)
      ;
  }

  displayMessage(screenContent);
}

void loop()
{
}