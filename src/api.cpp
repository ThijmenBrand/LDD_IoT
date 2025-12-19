#include "api.h"
#include "network.h"

const char *apiHost = "iot-ldd.thijmenbrand.nl";

String fetchScreenContent(String deviceId)
{
  String path = "/api/iot?deviceId=" + deviceId;
  HttpResponse result = httpsGet(apiHost, path);
  if (result.statusCode != 200)
  {
    Serial.print("API request failed with status code: ");
    Serial.println(result.statusCode);
    return "";
  }

  return result.body;
}