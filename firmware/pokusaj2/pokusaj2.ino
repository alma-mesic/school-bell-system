#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "time.h"
#include <Preferences.h>

Preferences prefs;  // globalni objekat za spremanje u flash

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
  String time;  // HH:MM
  String date;  // YYYY-MM-DD
};

Notification notifications[10];
int notificationCount = 0;

int currentNotifIndex = 0;
unsigned long lastNotifSwitch = 0;
const unsigned long notifInterval = 5000;  // 5 sekundi po obavijesti

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

// ================== RASPORED ==================
void checkSchedule() {
  String now = getTimeString();

  // 1. Aktivni čas
  bool classFound = false;
  for (int i = 0; i < classCount; i++) {
    if (now >= classes[i].start && now < classes[i].end) {
      text = String(classes[i].number) + ". cas u toku";
      classFound = true;
      break;
    }
  }
  if (!classFound) text = "Cekam raspored";

  // 2. Rotacija obavijesti za danas i sutra
  struct tm t;
  if (!getLocalTime(&t)) return;
  char buf[11];
  strftime(buf, 11, "%Y-%m-%d", &t);
  String today = String(buf);

  t.tm_mday += 1;  // sutrašnji datum
  mktime(&t);
  strftime(buf, 11, "%Y-%m-%d", &t);
  String tomorrow = String(buf);

  // Skupi sve relevantne obavijesti
  int relevantCount = 0;
  Notification relevant[10];

  for (int i = 0; i < notificationCount; i++) {
    if (notifications[i].date == today || notifications[i].date == tomorrow) {
      relevant[relevantCount++] = notifications[i];
    }
  }

  // Ako imamo relevantne obavijesti, rotiraj ih
  if (relevantCount > 0) {
    if (millis() - lastNotifSwitch > notifInterval) {
      currentNotifIndex = (currentNotifIndex + 1) % relevantCount;
      lastNotifSwitch = millis();
    }

    text += " | " + relevant[currentNotifIndex].text + " u " + relevant[currentNotifIndex].time;
  }

  x = 32;  // reset scroll kad se text promijeni
  text_width = text.length() * 6;
}


// ================== BRISANJE OBAVJESTI ==================
void removePastNotifications() {
  struct tm t;
  if (!getLocalTime(&t)) return;

  int nowHour = t.tm_hour;
  int nowMin = t.tm_min;
  char buf[11];
  strftime(buf, 11, "%Y-%m-%d", &t);
  String today = String(buf);

  bool changed = false;

  for (int i = 0; i < notificationCount;) {
    int notifHour = notifications[i].time.substring(0, 2).toInt();
    int notifMin = notifications[i].time.substring(3, 5).toInt();

    // briši samo ako je obavijest za danas i prošlo je vrijeme
    if (notifications[i].date == today && (notifHour < nowHour || (notifHour == nowHour && notifMin < nowMin))) {
      for (int j = i; j < notificationCount - 1; j++)
        notifications[j] = notifications[j + 1];
      notificationCount--;
      changed = true;
    } else {
      i++;
    }
  }

  // Ako smo obrisali neku obavijest, spremi novi niz u Preferences
  if (changed) {
    StaticJsonDocument<1024> doc;
    JsonArray arr = doc.createNestedArray("lista");
    for (int i = 0; i < notificationCount; i++) {
      JsonObject o = arr.createNestedObject();
      o["naziv"] = notifications[i].text;
      o["datumVrijeme"] = notifications[i].date + " " + notifications[i].time;  // <- ovdje ide
    }
    doc["tip"] = "obavijesti";

    String jsonStr;
    serializeJson(doc, jsonStr);
    saveData("obavijesti", jsonStr);
  }
}



// ================== EEPROM ==================
void saveData(const String& tip, const String& json) {
  prefs.putString(tip.c_str(), json);  // tip = "raspored" ili "obavijesti"
  Serial.println("Podaci sacuvani u Preferences: " + tip);
}

void loadSavedData() {
  // Raspored
  String rasporedJson = prefs.getString("raspored", "");
  if (rasporedJson.length() > 0) handleJson(rasporedJson);

  // Obavijesti
  String obavijestiJson = prefs.getString("obavijesti", "");
  if (obavijestiJson.length() > 0) handleJson(obavijestiJson);
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
    saveData("raspored", json);
  }

  else if (tip == "obavijesti") {
    notificationCount = 0;
    for (JsonObject o : doc["lista"].as<JsonArray>()) {
      String dt = o["datumVrijeme"].as<String>();  // format: YYYY-MM-DD HH:MM
      if (dt.length() >= 16) {
        notifications[notificationCount++] = {
          o["naziv"],
          dt.substring(11, 16),  // HH:MM
          dt.substring(0, 10)    // YYYY-MM-DD
        };
      }
    }
    saveData("obavijesti", json);
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

  prefs.begin("schoolbell", false);  // namespace: schoolbell, read/write
  loadSavedData();                   // učitaj prethodni raspored i obavijesti
}

// ================== LOOP ==================
void loop() {

  if (Serial.available())
    handleJson(Serial.readStringUntil('\n'));

  if (notificationCount > 0) {
    removePastNotifications();
  }

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
