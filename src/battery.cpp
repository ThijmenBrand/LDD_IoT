#include "battery.h"
#include <Arduino.h>

const float ADC_MAX_VALUE = (float)(1 << ADC_RESOLUTION);

const float REFERENCE_VOLTAGE = 3.3;          // Reference voltage for ADC
const float CHARGING_THRESHOLD_VOLTAGE = 4.1; // Voltage above which the battery is considered charging

// Start with 1.275 if utilizing the default MKR divider (1.2M/330k)
// You can tweak this later if the voltage is slightly off.
const float CALIBRATION_FACTOR = 1.3;

const float MIN_VOLTAGE = 3.2; // 0% (Cutoff safe zone)
const int SMOOTHING_SAMPLES = 10;

float getBatteryVoltage()
{
  // 1. Configure the ADC hardware to use 12 bits
  analogReadResolution(ADC_RESOLUTION);

  long rawSum = 0;
  for (int i = 0; i < SMOOTHING_SAMPLES; i++)
  {
    rawSum += analogRead(ADC_BATTERY);
    delay(2);
  }
  int raw = rawSum / SMOOTHING_SAMPLES;

  float voltage = raw * (REFERENCE_VOLTAGE / ADC_MAX_VALUE) * CALIBRATION_FACTOR;

  Serial.print("Raw ADC: ");
  Serial.print(raw);
  Serial.print(" | Calc Voltage: ");
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