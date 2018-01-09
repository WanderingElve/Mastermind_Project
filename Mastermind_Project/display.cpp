//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533

#include "display.h"

#include <Adafruit_ILI9341.h>

//This file contains the definition of tft

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 Display::tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
