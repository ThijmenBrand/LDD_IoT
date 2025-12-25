#include "battery.h"
#include <Arduino.h>

// CONFIGURATION FOR MKR WIFI 1010
// The board has an internal voltage divider connected to ADC_BATTERY
// R1 = 330k, R2 = 1.2M
// Factor = (R1 + R2) / R2 = (330 + 1200) / 1200 = 1.275
const float REFERENCE_VOLTAGE = 3.3;          // Reference voltage for ADC
const float CHARGING_THRESHOLD_VOLTAGE = 4.1; // Voltage above which the battery is considered charging
const float CALIBRATION_FACTOR = 3.33;

const float MIN_VOLTAGE = 3.2; // 0% (Cutoff safe zone)

const int SMOOTHING_SAMPLES = 10;

float getBatteryVoltage()
{
  analogReadResolution(ADC_RESOLUTION);

  long rawSum = 0;
  for (int i = 0; i < SMOOTHING_SAMPLES; i++)
  {
    rawSum += analogRead(ADC_BATTERY);
    delay(2);
  }
  int raw = rawSum / SMOOTHING_SAMPLES;

  float voltage = raw * (REFERENCE_VOLTAGE / ADC_RESOLUTION) * CALIBRATION_FACTOR;
  Serial.print("Bat Voltage: ");
  Serial.println(voltage);

  return voltage;
}

int getBatteryPercentage()
{
  float voltage = getBatteryVoltage();

  int percentage = (int)((voltage - MIN_VOLTAGE) * 100);

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