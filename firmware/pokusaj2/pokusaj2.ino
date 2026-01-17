#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "time.h"

// ================== PANEL ==================
#define PANEL_RES_X 32
#define PANEL_RES_Y 16
#define PANEL_CHAIN 1

MatrixPanel_I2S_DMA* display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);

// ================== BELL ==================
#define RELAY_PIN 18

// ================== GLOBAL ==================
bool startup = true;
unsigned long startTime;

int x = 32;
int16_t text_width;

bool bellTestMode = false;

// ================== SOS ==================
bool sosActive = false;
unsigned long sosStartTime = 0;
unsigned long sosBellTimer = 0;
int sosStep = 0;
String textBeforeSOS = "";

// Morse SOS: ... --- ...
const int sosPattern[] = {
  200, 200, 200, 600,
  600, 600, 600, 600,
  200, 200, 200
};
const int sosLen = 11;

// ================== TEXT ==================
String text = "Cekam raspored";

// ================== WIFI + TIME ==================
const char* ssid = "59588d";
const char* password = "273370344";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// ================== RASPORED ==================
struct ClassTime {
  int number;
  String start;
  String end;
};

ClassTime classes[20];
int classCount = 0;

// ================== OBAVIJESTI ==================
struct Notification {
  String text;
  String time;
};

Notification notifications[10];
int notificationCount = 0;

// ================== WIFI ==================
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

// ================== TIME ==================
String getTimeString() {
  struct tm t;
  if (!getLocalTime(&t)) return "00:00";
  char buf[6];
  strftime(buf, 6, "%H:%M", &t);
  return String(buf);
}

// ================== MATRIX CONFIG ==================
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
  mxconfig.gpio.clk = 16;
  mxconfig.gpio.lat = 4;
  mxconfig.gpio.oe = 15;
}

// ================== DISPLAY ==================
void showStartupScreen() {
  display->fillScreen(0);
  display->setTextColor(display->color565(255, 0, 0));
  display->setCursor(6, 4);
  display->print("ETS");
}

void showTime() {
  display->setTextColor(display->color565(252, 3, 148));
  display->setCursor(1, 0);
  display->print(getTimeString());
}

void scrollText() {
  display->setTextColor(display->color565(134, 50, 230));
  display->setCursor(x, 8);
  display->print(text);
  x--;
  if (x < -text_width) x = 32;
}

void drawMainScreen() {
  display->fillScreen(0);

  if (bellTestMode) {
    display->setTextColor(display->color565(255, 255, 0));
    display->setCursor(x, 4);
    display->print("TESTIRANJE ZVONA");
    x--;
    if (x < -32) x = 64;
  } else {
    showTime();
    scrollText();
  }
}

// ================== JSON ==================
void handleJson(String json) {
  StaticJsonDocument<2048> doc;
  if (deserializeJson(doc, json)) return;

  String tip = doc["tip"] | "";

  if (tip == "raspored") {
    classCount = 0;
    for (JsonObject c : doc["casovi"].as<JsonArray>()) {
      classes[classCount++] = {
        c["cas"], c["pocetak"], c["kraj"]
      };
    }
  }

  else if (tip == "obavijesti") {
    notificationCount = 0;
    for (JsonObject o : doc["lista"].as<JsonArray>()) {
      String dt = o["datumVrijeme"].as<String>();

      if (dt.length() >= 16) {
        notifications[notificationCount++] = {
          o["naziv"],
          dt.substring(11, 16)  // HH:MM
        };
      }
    }
  }

  else if (tip == "zvono") {
    String a = doc["akcija"];
    if (a == "start") {
      bellTestMode = true;
      digitalWrite(RELAY_PIN, HIGH);
    }
    if (a == "stop") {
      bellTestMode = false;
      digitalWrite(RELAY_PIN, LOW);
    }
  }

  else if (tip == "emergency") {
    textBeforeSOS = text;
    sosActive = true;
    sosStartTime = millis();
    sosBellTimer = millis();
    sosStep = 0;
  }
}

// ================== RASPORED ==================
void checkSchedule() {
  String now = getTimeString();

  for (int i = 0; i < classCount; i++) {
    if (now >= classes[i].start && now < classes[i].end) {
      text = String(classes[i].number) + ". cas u toku";

      if (notificationCount > 0)
        text += " | " + notifications[0].text + " u " + notifications[0].time;

      text_width = text.length() * 6;
      return;
    }
  }
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  configuration();

  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);
  display->setTextWrap(false);

  connectWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  startTime = millis();
  text_width = text.length() * 6;
}

// ================== LOOP ==================
void loop() {

  if (Serial.available())
    handleJson(Serial.readStringUntil('\n'));

  if (startup) {
    showStartupScreen();
    if (millis() - startTime > 3000) startup = false;
    return;
  }

  if (sosActive) {
    display->fillScreen(
      ((millis() / 300) % 2) ? 0 : display->color565(255, 0, 0));

    if (millis() - sosBellTimer > sosPattern[sosStep]) {
      sosBellTimer = millis();
      digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
      sosStep++;
    }

    if (millis() - sosStartTime > 10000) {
      sosActive = false;
      digitalWrite(RELAY_PIN, LOW);
      text = textBeforeSOS;
    }
    return;
  }

  checkSchedule();
  drawMainScreen();
  delay(80);
}
