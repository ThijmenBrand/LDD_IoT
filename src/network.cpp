#include "network.h"
#include <ArduinoHttpClient.h>
#include "../lib/secrets.h"

WiFiSSLClient wifiClient;

const int WIFI_TIMEOUT_MS = 20000;

int connectWifi()
{
  if (WiFi.status() == WL_CONNECTED)
    return WIFI_CONNECTED;

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  const unsigned long timeoutMs = 20000; // 20 seconds
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return WIFI_CONNECTED;
  }
  else
  {
    Serial.println("");
    Serial.println("WiFi connection failed.");
    return WIFI_ERROR;
  }
}

HttpResponse httpsGet(const char *host, String path)
{
  Serial.print("Requesting: ");
  Serial.print(host);
  Serial.println(path);

  HttpClient client = HttpClient(wifiClient, host, 443);
  client.beginRequest();
  client.get(path);
  client.sendHeader("X-NYUSZI-SECRET", DEVICE_SECRET);
  client.beginBody();
  client.endRequest();

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