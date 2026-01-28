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

String adminPassword = "1234";  // isto kao u admin.json
String lastText = "";


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
  int year;
  int month;
  int day;
  int hour;
  int minute;
};

int dayOfYear(int y, int m, int d) {
  struct tm t = {};
  t.tm_year = y - 1900;
  t.tm_mon  = m - 1;
  t.tm_mday = d;
  mktime(&t);
  return t.tm_yday;
}

bool isTodayOrTomorrow(Notification &n, struct tm &now) {
  int notifDay = dayOfYear(n.year, n.month, n.day);
  return notifDay == now.tm_yday || notifDay == now.tm_yday + 1;
}


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

int timeToMinutes(const String& t) {
  int h = t.substring(0, 2).toInt();
  int m = t.substring(3, 5).toInt();
  return h * 60 + m;
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
  display->setTextSize(1);
  display->setTextWrap(false);

  if (bellTestMode) {
    display->setTextColor(display->color565(255, 255, 0));
    display->setCursor(x, 4);
    display->print("TESTIRANJE ZVONA");

    x--;
    if (x < -100) x = display->width();
  } else {
    showTime();
    scrollText();
  }
}


// ================== RASPORED ==================
void buildMainText() {
  struct tm now;
  if (!getLocalTime(&now)) return;

  String newText = "";

  // Cas u toku
String nowTime = getTimeString();
int nowMin = now.tm_hour * 60 + now.tm_min;
bool classInProgress = false;

for (int i = 0; i < classCount; i++) {
  int startMin = timeToMinutes(classes[i].start);
  int endMin   = timeToMinutes(classes[i].end);

  if (nowMin >= startMin && nowMin < endMin) {
    int remaining = endMin - nowMin;

    newText =
      String(classes[i].number) + ". cas u toku, zavrsava se u " +
      classes[i].end +
      ", ostalo jos " +
      remaining + " min";

    classInProgress = true;
    break;
  }
}


  // Ako nema časa u toku, proveri da li je pauza između dva časa (5 min)
  if (!classInProgress && classCount > 0) {
    for (int i = 0; i < classCount - 1; i++) {
      // trenutni kraj časa
      String kraj = classes[i].end;
      int krajH = kraj.substring(0,2).toInt();
      int krajM = kraj.substring(3,5).toInt();
      int krajMin = krajH*60 + krajM;

      // sledeći početak časa
      String start = classes[i+1].start;
      int startH = start.substring(0,2).toInt();
      int startM = start.substring(3,5).toInt();
      int startMin = startH*60 + startM;

      int sadaMin = now.tm_hour*60 + now.tm_min;

      // proveri da li smo u pauzi između dva časa
      if (sadaMin >= krajMin && sadaMin < startMin) {
        newText = "ODMOR";
        break;
      }
    }
  }

  if (newText == "") newText = "Nema casa";

  // Današnji datum
  int todayYear  = now.tm_year + 1900;
  int todayMonth = now.tm_mon + 1;
  int todayDay   = now.tm_mday;

  // Sutrašnji datum
  time_t t_now = mktime(&now) + 24*3600;
  struct tm tm_tomorrow;
  localtime_r(&t_now, &tm_tomorrow);
  int tomorrowYear  = tm_tomorrow.tm_year + 1900;
  int tomorrowMonth = tm_tomorrow.tm_mon + 1;
  int tomorrowDay   = tm_tomorrow.tm_mday;

  // Obavijesti
  for (int i = 0; i < notificationCount; i++) {
    bool isToday = notifications[i].year == todayYear &&
                   notifications[i].month == todayMonth &&
                   notifications[i].day == todayDay;

    bool isTomorrow = notifications[i].year == tomorrowYear &&
                      notifications[i].month == tomorrowMonth &&
                      notifications[i].day == tomorrowDay;

    if (!isToday && !isTomorrow) continue;

    newText += " | ";
    if (isTomorrow) newText += "sutra ";

    newText += notifications[i].text;
    newText += " u ";

    if (notifications[i].hour < 10) newText += "0";
    newText += notifications[i].hour;
    newText += ":";

    if (notifications[i].minute < 10) newText += "0";
    newText += notifications[i].minute;
  }

  // Reset scrolla samo ako se promijenio tekst
  if (newText != lastText) {
    text = newText;
    lastText = newText;
    text_width = text.length() * 6;
    x = 32;
  }
}


// ================== ROTACIJA OBAVJESTI ==================
void rotateNotifications() {
  if (notificationCount == 0) return;

  if (millis() - lastNotifSwitch > notifInterval) {
    currentNotifIndex++;
    if (currentNotifIndex >= notificationCount)
      currentNotifIndex = 0;
    lastNotifSwitch = millis();
  }
}

// ================== BRISANJE OBAVJESTI ==================
void removePastNotifications() {
  struct tm now;
  if (!getLocalTime(&now)) return;

  bool changed = false;

  for (int i = 0; i < notificationCount;) {
    // ako NIJE danas → preskoči
    if (notifications[i].year != now.tm_year + 1900 ||
        notifications[i].month != now.tm_mon + 1 ||
        notifications[i].day != now.tm_mday) {
      i++;
      continue;
    }

    // ako je danas, ali vrijeme još NIJE prošlo → preskoči
    if (notifications[i].hour > now.tm_hour ||
        (notifications[i].hour == now.tm_hour &&
         notifications[i].minute > now.tm_min)) {
      i++;
      continue;
    }

    // prošla obavijest → briši
    for (int j = i; j < notificationCount - 1; j++) {
      notifications[j] = notifications[j + 1];
    }
    notificationCount--;
    changed = true;
  }

  // Snimi novo stanje u Preferences
  if (changed) {
    StaticJsonDocument<1024> doc;
    JsonArray arr = doc.createNestedArray("lista");

    for (int i = 0; i < notificationCount; i++) {
      JsonObject o = arr.createNestedObject();
      o["naziv"] = notifications[i].text;

      char buf[20];
      sprintf(buf, "%04d-%02d-%02d %02d:%02d",
              notifications[i].year,
              notifications[i].month,
              notifications[i].day,
              notifications[i].hour,
              notifications[i].minute);
      o["datumVrijeme"] = buf;
    }

    doc["tip"] = "obavijesti";

    String json;
    serializeJson(doc, json);
    saveData("obavijesti", json);
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

void clearEEPROM() {
  prefs.clear();  // briše SVE iz namespace "schoolbell"
  Serial.println("EEPROM obrisan!");

  // reset stanja u RAM-u
  classCount = 0;
  notificationCount = 0;
  text = "EEPROM obrisan";
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
    String dt = o["datumVrijeme"]; // YYYY-MM-DD HH:MM

    if (dt.length() < 16 || notificationCount >= 10) continue;

    notifications[notificationCount++] = {
      o["naziv"].as<String>(),
      dt.substring(0,4).toInt(),   // year
      dt.substring(5,7).toInt(),   // month
      dt.substring(8,10).toInt(),  // day
      dt.substring(11,13).toInt(), // hour
      dt.substring(14,16).toInt()  // minute
    };
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

  else if (tip == "clear_eeprom") {
    if (doc["password"] == adminPassword)
      clearEEPROM();
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

  static unsigned long lastCleanup = 0;

  if (millis() - lastCleanup > 60000) {  // svaku 1 min
    removePastNotifications();
    lastCleanup = millis();
  }

  if (startup) {
    showStartupScreen();
    if (millis() - startTime > 3000) startup = false;
    return;
  }

  if (sosActive) {
    display->fillScreen(((millis() / 300) % 2) ? 0 : display->color565(255, 0, 0));

    if (millis() - sosBellTimer > sosPattern[sosStep]) {
      sosBellTimer = millis();
      digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
      sosStep = (sosStep + 1) % sosLen;
    }

    if (millis() - sosStartTime > 10000) {
      sosActive = false;
      digitalWrite(RELAY_PIN, LOW);
      text = textBeforeSOS;
    }
    return;
  }
  buildMainText();
  drawMainScreen();
  delay(80);
}
