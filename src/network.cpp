#include "network.h"
#include <WiFiNINA.h>
#include "../lib/secrets.h"

WiFiSSLClient wifiClient;

int connectWifi()
{
  if (WiFi.status() == WL_CONNECTED)
    return WIFI_CONNECTED;

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  return WIFI_CONNECTED;
}