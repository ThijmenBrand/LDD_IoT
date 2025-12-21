#include "battery.h"
#include <Arduino.h>

// CONFIGURATION FOR MKR WIFI 1010
// The board has an internal voltage divider connected to ADC_BATTERY
// R1 = 330k, R2 = 1.2M
// Factor = (R1 + R2) / R2 = (330 + 1200) / 1200 = 1.275
const float VOLTAGE_DIVIDER_RATIO = 1.275;
const float REFERENCE_VOLTAGE = 3.3; // Reference voltage for ADC

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

  // Mapping Strategy:
  // 4.15V+  -> 100% (Fully Charged)
  // 3.90V   -> 70%
  // 3.70V   -> 20% (Nominal voltage)
  // 3.50V   -> 0%  (Cutoff safe zone)

  int percentage = 0;
  if (voltage >= 4.15)
  {
    percentage = 100;
  }
  else if (voltage >= 3.90)
  {
    // Linear interpolation between 3.9 and 4.15
    percentage = 70 + (int)((voltage - 3.90) * (30 / 0.25)); // 70% to 100%
  }
  else if (voltage >= 3.70)
  {
    // Linear interpolation between 3.7 and 3.9
    percentage = 20 + (int)((voltage - 3.70) * (50 / 0.20)); // 20% to 70%
  }
  else if (voltage >= 3.50)
  {
    // Linear interpolation between 3.5 and 3.7
    percentage = (int)((voltage - 3.50) * (20 / 0.20)); // 0% to 20%
  }
  else
  {
    // Below 3.5V is considered 0%
    percentage = 0; // Below safe voltage
  }

  if (percentage > 100)
    percentage = 100;
  if (percentage < 0)
    percentage = 0;

  return percentage;
}
