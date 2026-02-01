#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>

void drawErrorScreen(String errorMessage, GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> &display);