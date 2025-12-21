#include "network.h"
#include <ArduinoHttpClient.h>
#include "../lib/secrets.h"

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

HttpResponse httpsGet(const char *host, String path)
{
  Serial.print("Requesting: ");
  Serial.print(host);
  Serial.println(path);

  HttpClient client = HttpClient(wifiClient, host, 443);
  client.get(path);

  int statusCode = client.responseStatusCode();
  if (statusCode != 200)
  {
    Serial.print("HTTP Error: ");
    Serial.println(statusCode);
    return HttpResponse{statusCode, 0, ""};
  }

  String body = client.responseBody();
  int contentLength = body.length();

  client.stop();
  return HttpResponse{statusCode, contentLength, body};
}