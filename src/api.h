#ifndef API_H
#define API_H

#include <Arduino.h>

struct ApiResponse
{
  String widget;
  String rawData;
};

ApiResponse fetchScreenContent(String deviceId);

#endif