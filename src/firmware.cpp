#include "firmware.h"
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "network.h"
#include "../lib/secrets.h"

#define NO_GLOBAL_ARDUINOOTA
#include <ArduinoOTA.h>

extern WiFiSSLClient wifiClient;

const char *fwHost = "raw.githubusercontent.com";
const int fwPort = 443;

int checkFirmwareUpdate()
{
  Serial.println("Checking for firmware update...");

  String versionPath = String("/") + GH_USER + "/" + GH_REPO + "/" + GH_BRANCH + "/firmware.txt?t=" + String(millis());

  HttpClient versionClient = HttpClient(wifiClient, fwHost, fwPort);
  versionClient.get(versionPath);

  int statusCode = versionClient.responseStatusCode();
  if (statusCode != 200)
  {
    Serial.print("Failed to fetch firmware version. HTTP Status Code: ");
    Serial.println(statusCode);
    versionClient.stop();
    return COULD_NOT_FETCH_VERSION;
  }

  String response = versionClient.responseBody();
  int latestFirmwareVersion = response.toInt();
  Serial.print("Server firmware version: ");
  Serial.println(latestFirmwareVersion);
  Serial.print("Current firmware version: ");
  Serial.println(CURRENT_VERSION);

  if ((int)latestFirmwareVersion <= (int)CURRENT_VERSION)
  {
    Serial.println("Firmware is up to date.");
    return FIRMWARE_UP_TO_DATE;
  }

  Serial.println("New firmware version available. Starting update...");

  versionClient.stop();

  return NEEDS_UPDATE;
}

int updateFirmware()
{
  String binPath = String("/") + GH_USER + "/" + GH_REPO + "/" + GH_BRANCH + "/firmware.bin?t=" + String(millis());

  HttpClient firmwareClient = HttpClient(wifiClient, fwHost, fwPort);
  firmwareClient.get(binPath);

  if (firmwareClient.responseStatusCode() != 200)
  {
    Serial.println("Failed to download firmware binary.");
    firmwareClient.stop();
    return COULD_NOT_DOWNLOAD_BINARY;
  }

  long contentLength = firmwareClient.contentLength();
  if (contentLength <= 0)
  {
    Serial.println("Invalid content length.");
    firmwareClient.stop();
    return INVALID_CONTENT_LENGTH;
  }

  if (!InternalStorage.open((int)contentLength))
  {
    Serial.println("Not enough space for firmware update.");
    firmwareClient.stop();
    return NOT_ENOUGH_SPACE;
  }

  byte buffer[128];
  while (firmwareClient.connected() || firmwareClient.available())
  {
    if (firmwareClient.available())
    {
      int bytesRead = firmwareClient.read(buffer, sizeof(buffer));
      if (bytesRead > 0)
      {
        for (int i = 0; i < bytesRead; i++)
        {
          InternalStorage.write(buffer[i]);
        }
      }
    }
  }

  InternalStorage.close();
  firmwareClient.stop();

  Serial.println("Update Downloaded. Rebooting to apply update...");
  InternalStorage.apply();

  return UPDATE_SUCCESSFUL;
}