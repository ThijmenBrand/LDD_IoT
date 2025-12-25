#ifndef NETWORK_H
#define NETWORK_H

#define WIFI_ERROR 1
#define WIFI_CONNECTED 0

#include <Arduino.h>
#include <WiFiNINA.h>

extern WiFiSSLClient wifiClient;

struct HttpResponse
{
  int statusCode;
  int contentLength;
  String body;
};

int connectWifi();
HttpResponse httpsGet(const char *host, String path);

#endif // NETWORK_H