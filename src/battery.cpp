#include "battery.h"
#include <Arduino.h>

// CONFIGURATION FOR MKR WIFI 1010
// The board has an internal voltage divider connected to ADC_BATTERY
// R1 = 330k, R2 = 1.2M
// Factor = (R1 + R2) / R2 = (330 + 1200) / 1200 = 1.275
const float VOLTAGE_DIVIDER_RATIO = 1.275;
const float REFERENCE_VOLTAGE = 3.3;          // Reference voltage for ADC
const float CHARGING_THRESHOLD_VOLTAGE = 4.1; // Voltage above which the battery is considered charging

const int MAX_DC_VALUE = (1 << ADC_RESOLUTION) - 1; // 4095 for 12-bit ADC

const float MIN_VOLTAGE = 3.2; // 0% (Cutoff safe zone)
const float MAX_VOLTAGE = 4.2; // 100% (Fully Charged)

float getBatteryVoltage()
{
  analogReadResolution(ADC_RESOLUTION);
  int rawADC = analogRead(ADC_BATTERY);

  float volgateAtPin = rawADC * (REFERENCE_VOLTAGE / MAX_DC_VALUE);
  float batteryVoltage = volgateAtPin * VOLTAGE_DIVIDER_RATIO;

  return batteryVoltage;
}

int getBatteryPercentage()
{
  float voltage = getBatteryVoltage();

  int percentage = (int)((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE) * 100);

  // Clamp 0-100
  if (percentage > 100)
    percentage = 100;
  if (percentage < 0)
    percentage = 0;
  return percentage;
}

bool isCharging()
{
  return getBatteryVoltage() > CHARGING_THRESHOLD_VOLTAGE;
}