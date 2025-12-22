#include "api.h"
#include "network.h"
#include <ArduinoJson.h>

const char *apiHost = "iot-ldd.thijmenbrand.nl";

ApiResponse fetchScreenContent(String deviceId)
{
  ApiResponse apiResponse;
  apiResponse.widget = "error";

  String path = "/api/iot?deviceId=" + deviceId;
  HttpResponse result = httpsGet(apiHost, path);
  if (result.statusCode != 200)
  {
    Serial.print("API request failed with status code: ");
    Serial.println(result.statusCode);
    return apiResponse;
  }

  JsonDocument doc;

  Serial.println("API response: " + result.body);

  DeserializationError error = deserializeJson(doc, result.body);

  if (!error)
  {
    apiResponse.widget = doc["widget"].as<String>();
    // Serialize the inner 'data' object back to a string string to pass it along
    serializeJson(doc["data"], apiResponse.rawData);
  }
  else
  {
    apiResponse.widget = "error";
    apiResponse.rawData = "JSON Fail";
  }

  return apiResponse;
}