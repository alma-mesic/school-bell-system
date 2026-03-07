#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include "time.h"

// ================= PANEL =================
#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 2

MatrixPanel_I2S_DMA *display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);


// ================= WIFI ==================
const char* ssid = "59588d";
const char* password = "273370344";

// ================= TIME ==================
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// ================= TEXT ==================
String statusText = "NEMA CASA";

// ================= LOGO ==================
const uint16_t logo[32 * 32] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x25, 0x55, 0x00, 0x02, 0x95, 0x22, 0x00, 
	0x04, 0x45, 0x54, 0x00, 0x02, 0x2a, 0xa9, 0x40, 0x05, 0x4a, 0x45, 0x00, 0x04, 0x22, 0x94, 0x80, 
	0x02, 0x94, 0xa9, 0x00, 0x09, 0x52, 0xa4, 0xa0, 0x04, 0x2f, 0xf2, 0x00, 0x02, 0xde, 0xb9, 0x50, 
	0x09, 0x7f, 0xfa, 0x40, 0x04, 0xbd, 0x79, 0x00, 0x02, 0x7b, 0xed, 0x50, 0x04, 0xbe, 0xbc, 0x80, 
	0x02, 0x5b, 0xfe, 0x00, 0x01, 0x3e, 0x81, 0x40, 0x04, 0x9a, 0x54, 0x80, 0x01, 0x4a, 0x89, 0x00, 
	0x00, 0x25, 0x24, 0x00, 0x01, 0x5a, 0xaa, 0x00, 0x00, 0x4a, 0x51, 0x00, 0x00, 0x21, 0x08, 0x00, 
	0x00, 0x5d, 0x64, 0x00, 0x00, 0x02, 0x90, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x02, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// ================= CONFIG =================
void configuration() {

  mxconfig.gpio.r1 = 25;
  mxconfig.gpio.g1 = 27;
  mxconfig.gpio.b1 = 26;

  mxconfig.gpio.r2 = 14;
  mxconfig.gpio.g2 = 13;
  mxconfig.gpio.b2 = 12;

  mxconfig.gpio.a = 23;
  mxconfig.gpio.b = 19;
  mxconfig.gpio.c = 5;
  mxconfig.gpio.d = 17;

  mxconfig.gpio.clk = 16;
  mxconfig.gpio.lat = 4;
  mxconfig.gpio.oe = 15;
}

// ================= WIFI CONNECT =================
void connectWiFi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// ================= TIME STRING =================
String getTimeString() {

  struct tm t;

  if (!getLocalTime(&t))
    return "00:00";

  char buf[6];
  strftime(buf, 6, "%H:%M", &t);

  return String(buf);
}

// ================= DRAW SCREEN =================
void drawScreen() {

  display->clearScreen();

  // ===== LOGO =====
  display->drawRGBBitmap(0,0,logo,32,32);

  // ===== TIME =====
  display->setTextSize(2);
  display->setTextColor(display->color565(255, 255, 255));

  display->setCursor(40, 5);
  display->print(getTimeString());

  // ===== STATUS TEXT =====
  display->setTextSize(1);
  display->setTextColor(display->color565(255, 0, 0));

  display->setCursor(40, 24);
  display->print(statusText);
}

// ================= SETUP =================
void setup() {

  configuration();

  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);
  display->setTextWrap(false);

  connectWiFi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

// ================= LOOP =================
void loop() {

  drawScreen();

  delay(1000);
}