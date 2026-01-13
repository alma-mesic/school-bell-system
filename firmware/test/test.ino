#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include "time.h"
#include <WiFi.h>

#define PANEL_RES_X 32
#define PANEL_RES_Y 16
#define PANEL_CHAIN 1

MatrixPanel_I2S_DMA *display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);

bool startup = true;
unsigned long startTime;

String text = "sedmi cas u toku";
int x = 32;

// WiFi i NTP
const char* ssid = "59588d";
const char* password = "273370344";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Povezivanje na WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Povezano!");
}

String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "00:00";
  }
  char buffer[6];
  strftime(buffer, 6, "%H:%M", &timeinfo);
  return String(buffer);
}

void setup() {
  Serial.begin(115200);

  // Pinovi
  mxconfig.gpio.r1 = 25; mxconfig.gpio.g1 = 27; mxconfig.gpio.b1 = 26;
  mxconfig.gpio.r2 = 14; mxconfig.gpio.g2 = 13; mxconfig.gpio.b2 = 12;
  mxconfig.gpio.a = 23; mxconfig.gpio.b = 19; mxconfig.gpio.c = 5;
  mxconfig.gpio.clk = 16; mxconfig.gpio.lat = 4; mxconfig.gpio.oe = 15;

  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);
  display->clearScreen();

  startTime = millis();

  // Wi-Fi + NTP
  connectWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // čekaj dok NTP ne bude spreman
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)) {
    delay(500);
    Serial.println("Čekam NTP...");
  }
}

void loop() {
  // ----- STARTUP ETS 10s -----
  if (startup) {
    display->clearScreen();
    display->setTextSize(1);
    display->setTextColor(display->color565(255,0,0));
    display->setCursor(6,4);
    display->print("ETS");

    // Produženo na 10 sekundi
    if (millis() - startTime > 10000) {
      startup = false;
      x = 32; // reset scroll
    }
    delay(100);
    return;
  }

  // ----- Nakon startup, kombinovani prikaz -----
  display->clearScreen();

  // Vrijeme
  display->setTextSize(1);
  display->setTextColor(display->color565(0,255,0));
  display->setCursor(0,0);
  display->print(getTimeString());

  // Scroll obavještenje
  display->setTextSize(1);
  display->setTextColor(display->color565(255,255,0));
  display->setCursor(x,10);
  display->print(text);

  // pomak scrolla
  x--;
  if (x < -(text.length() * 6)) x = 32; // reset kad prođe

  delay(150); // malo sporije scrollanje
}
