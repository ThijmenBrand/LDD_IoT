#include <Arduino.h>
#include "network.h"
#include "firmware.h"
#include "display.h"
#include "battery.h"
#include "api.h"
#include <ArduinoLowPower.h>

// DISPLAY SETTINGS
const int MAX_HEIGHT = 130;
const int MAX_WIDHT = 300;

const String DEVICE_ID = "e74010ab-4799-462f-b4b8-942903485c41";
const int UPDATE_INTERVAL_MS = 3600000; // 1 Hour

void setup()
{
  Serial.begin(115200);
  initDisplay();

  int batteryLevel = getBatteryPercentage();
  Serial.println("Battery Level: " + String(batteryLevel) + "%");
  if (batteryLevel <= 5)
  {
    Serial.println("Battery critically low. Showing empty battery screen.");
    showEmptyBatteryScreen();

    WiFi.end();

    LowPower.deepSleep(1800000);

    NVIC_SystemReset();
  }

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

  if (batteryLevel < 20 && !isCharging())
  {
    displayMessage("Charge battery" + String(batteryLevel) + "%", true);
  }
  else
  {
    displayMessage("Battery: " + String(batteryLevel) + "%", true);
  }

  delay(3000);

  displayMessage("Syncing...", 100, 50);

  ApiResponse data = fetchScreenContent(DEVICE_ID);
  Serial.println("Battery Level for display: " + String(batteryLevel) + "%");
  drawScreen(data.rawData, batteryLevel, batteryLevel <= 20, data.widget);

  Serial.println("Update complete. Sleeping for 1 hour...");
  WiFi.end();
  LowPower.deepSleep(UPDATE_INTERVAL_MS);
  NVIC_SystemReset();
}

void loop()
{
}