#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "../lib/firmware.h"
#include "../lib/secrets.h"

#define NO_GLOBAL_ARDUINOOTA
#include <ArduinoOTA.h>

// DISPLAY SETTINGS
const int MAX_HEIGHT = 130;
const int MAX_WIDHT = 300;

const int EINK_BUSY = 7;
const int EINK_RST = 6;
const int EINK_DC = 5;
const int EINK_CS = 4;

GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(EINK_CS, EINK_DC, EINK_RST, EINK_BUSY));

int firmwareVersion = 1;
const char* fwHost = "raw.githubusercontent.com";
const int fwPort = 443;

WiFiSSLClient wifiClient;

void connectWifi();
void checkFirmwareUpdate();
void updateFirmware(String binaryPath);
void displayMessage(String message);

void setup() {
  Serial.begin(115200);

  // Initialize the e-paper
  display.init(115200);
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  displayMessage("Starting up...\nv" + String(CURRENT_VERSION));

  delay(5000);

  connectWifi();

  checkFirmwareUpdate();

  // Show connecting message
  display.setFullWindow();
  display.firstPage();
do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.print("Baby nyuszi\nFirmware v" + String(CURRENT_VERSION) + "\n\nConnected to WiFi!\nIP:\n" + WiFi.localIP().toString());
  } while (display.nextPage());
}

void loop() {
 if (Serial.available() > 0) {
  char command = Serial.read();

  if (command == 'u') {
      Serial.println("Manual update check triggered.");
      checkFirmwareUpdate();
    }
 }
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void checkFirmwareUpdate() {
  Serial.println("Checking for firmware update...");

  String versionPath = String("/") + GH_USER + "/" + GH_REPO + "/" + GH_BRANCH + "/firmware.txt?t=" + String(millis());

  HttpClient versionClient = HttpClient(wifiClient, fwHost, fwPort);
  versionClient.get(versionPath);

  int statusCode = versionClient.responseStatusCode();
  if (statusCode != 200) {
    Serial.print("Failed to fetch firmware version. HTTP Status Code: ");
    Serial.println(statusCode);
    versionClient.stop();
    return;
  }

  String response = versionClient.responseBody();
  int latestFirmwareVersion = response.toInt();
  Serial.print("Server firmware version: "); Serial.println(latestFirmwareVersion);
  Serial.print("Current firmware version: "); Serial.println(CURRENT_VERSION);

  if ((int)latestFirmwareVersion <= (int)CURRENT_VERSION) {
    Serial.println("Firmware is up to date.");
    return;
  }

  Serial.println("New firmware version available. Starting update...");
  displayMessage("Updating firmware...\n Do not unplug!\n v" + String(latestFirmwareVersion));

  String binPath = String("/") + GH_USER + "/" + GH_REPO + "/" + GH_BRANCH + "/firmware.bin?t=" + String(millis());

  versionClient.stop();

  updateFirmware(binPath);
}

void updateFirmware(String binaryPath) {
  HttpClient firmwareClient = HttpClient(wifiClient, fwHost, fwPort);
  firmwareClient.get(binaryPath);

  if (firmwareClient.responseStatusCode() != 200) {
    Serial.println("Failed to download firmware binary.");
    firmwareClient.stop();
    return;
  }

  long contentLength = firmwareClient.contentLength();
  if (contentLength <= 0) {
    Serial.println("Invalid content length.");
    firmwareClient.stop();
    return;
  }

  if (!InternalStorage.open((int)contentLength)) {
    Serial.println("Not enough space for firmware update.");
    firmwareClient.stop();
    return;
  }

  byte buffer[128];
  while (firmwareClient.connected() || firmwareClient.available()) {
    if (firmwareClient.available()) {
      int bytesRead = firmwareClient.read(buffer, sizeof(buffer));
      if (bytesRead > 0) {
        for (int i = 0; i < bytesRead; i++) {
          InternalStorage.write(buffer[i]);
        }
      }
    }
  }

  InternalStorage.close();
  firmwareClient.stop();

  Serial.println("Update Downloaded. Rebooting to apply update...");
  InternalStorage.apply();
}

void displayMessage(String message) {
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.print(message);
  } while (display.nextPage());
}