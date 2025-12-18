#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <Arduino.h>

#define COULD_NOT_FETCH_VERSION 1
#define COULD_NOT_DOWNLOAD_BINARY 2
#define INVALID_CONTENT_LENGTH 3
#define NOT_ENOUGH_SPACE 4
#define UPDATE_SUCCESSFUL 5
#define FIRMWARE_UP_TO_DATE 6
#define NEEDS_UPDATE 7

int checkFirmwareUpdate();
int updateFirmware();

#endif // FIRMWARE_H