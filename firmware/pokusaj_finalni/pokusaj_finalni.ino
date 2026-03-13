#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "time.h"
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#include "LittleFS.h"

// ---------------- KONFIGURACIJA ---------------------
#define RELAY_PIN 18
#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 2

#define LED_PIN 32     // broj pina led trake
#define LED_COUNT 200  // broj ledica na traci

MatrixPanel_I2S_DMA *display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
Preferences prefs;
AsyncWebServer server(80);

// ---------------- GLOBALNE PROMJENLJIVE --------------------
bool startup = true;
unsigned long startTime;
int xPos = 128;

bool bellTestMode = false;
int lastBellMinute = -1;
String text = "Cekam raspored...";
String lastText = "";

// SOS / Emergency
bool sosActive = false;
unsigned long sosStartTime = 0;
unsigned long sosBellTimer = 0;
int sosStep = 0;
const int sosPattern[] = { 200, 200, 200, 600, 600, 600, 600, 600, 200, 200, 200 };
const int sosLen = 11;

// Podaci
struct ClassTime {
  String day;
  int number;
  String start;
  String end;
  String dezurni;
};
ClassTime classes[20];
int classCount = 0;

struct Notification {
  String text;
  int year, month, day, hour, minute;
};
Notification notifications[20];
int notificationCount = 0;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int ledR = 255, ledG = 255, ledB = 255;  // Boja LED trake
int satR = 255, satG = 0, satB = 0;      // Boja sata na matrici
int textR = 0, textG = 255, textB = 0;   // Boja scroll teksta


// ---------------- POMOĆNE FUNKCIJE --------------------

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

String getTimeString() {
  struct tm t;
  if (!getLocalTime(&t)) return "00:00";
  char buf[6];
  strftime(buf, 6, "%H:%M", &t);
  return String(buf);
}

int timeToMinutes(const String &t) {
  return t.substring(0, 2).toInt() * 60 + t.substring(3, 5).toInt();
}

// ---------------- WEB SERVER RUTE (ZA WEB APP) --------------------

void handleJson(String json) {
  StaticJsonDocument<4096> doc;
  if (deserializeJson(doc, json)) return;

  String tip = doc["tip"] | "";

  if (tip == "raspored") {
    classCount = 0;
    for (JsonObject c : doc["casovi"].as<JsonArray>()) {
      if (classCount < 20) {
        int broj = c["cas"] | c["redniBroj"] | 0;
        classes[classCount++] = { c["dan"] | "", c["cas"].as<int>() | 0, c["pocetak"] | "", c["kraj"] | "", c["dezurni"] | "" };
      }
    }
    prefs.putString("raspored", json);
  } else if (tip == "obavijesti") {
    notificationCount = 0;
    for (JsonObject o : doc["lista"].as<JsonArray>()) {
      String dt = o["datumVrijeme"] | "";
      if (dt.length() >= 16 && notificationCount < 20) {
        notifications[notificationCount++] = {
          o["naziv"] | "", dt.substring(0, 4).toInt(), dt.substring(5, 7).toInt(),
          dt.substring(8, 10).toInt(), dt.substring(11, 13).toInt(), dt.substring(14, 16).toInt()
        };
      }
    }
    prefs.putString("obavijesti", json);
  } else if (tip == "emergency") {
    sosActive = true;
    sosStartTime = millis();
    sosStep = 0;
  } else if (tip == "zvono") {
    String akcija = doc["akcija"] | "";
    Serial.print("Primljena komanda za zvono: ");
    Serial.println(akcija);
    if (akcija == "start") {
      bellTestMode = true;
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      bellTestMode = false;
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}

void setupRoutes() {
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) request->send(200);
    else request->send(404);
  });

  // Ruta koja prima JSON iz Web App i prosljeđuje ga tvojoj handleJson funkciji
  server.on(
    "/api/data", HTTP_POST, [](AsyncWebServerRequest *request) {
      request->send(200, "application/json", "{\"status\":\"ok\"}");
    },
    NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      String body = "";
      for (size_t i = 0; i < len; i++) body += (char)data[i];
      handleJson(body);
    });
  server.on(
    "/api/settings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<256> doc;
      deserializeJson(doc, (const char *)data);

      String naredba = doc["naredba"];
      String tip = doc["tip"];
      int r = doc["r"];
      int g = doc["g"];
      int b = doc["b"];

      if (naredba == "SET_COLOR") {
        if (tip == "led_traka") {
          for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(r, g, b));
          }
          strip.show();
        } else if (tip == "boja_sata") {
          satR = r;
          satG = g;
          satB = b;
          prefs.putInt("satR", r);
          prefs.putInt("satG", g);
          prefs.putInt("satB", b);
        } else if (tip == "boja_scroll") {
          textR = r;
          textG = g;
          textB = b;
          prefs.putInt("textR", r);
          prefs.putInt("textG", g);
          prefs.putInt("textB", b);
        }

        request->send(200, "application/json", "{\"status\":\"ok\"}");
      }

      if (naredba == "CLEAR_EEPROM") {

        prefs.clear();  // briše sve iz NVS

        classCount = 0;
        notificationCount = 0;

        text = "Cekam raspored...";
        lastText = "";
        xPos = 128;

        request->send(200, "application/json", "{\"status\":\"ok\"}");

        Serial.println("Memorija obrisana");
        ESP.restart();
      }
    });
  server.on(
    "/api/profile", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      StaticJsonDocument<256> doc;
      deserializeJson(doc, (const char *)data);

      String naredba = doc["naredba"];

      if (naredba == "UPDATE_USER") {
        String noviUser = doc["novo"];
        prefs.putString("adminUser", noviUser);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
      } else if (naredba == "UPDATE_PASS") {
        String novaSifra = doc["nova"];
        prefs.putString("adminPass", novaSifra);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
      } else if (naredba == "UPDATE_WIFI") {
        String ssid = doc["ssid"];
        String pass = doc["pass"];
        prefs.putString("wifi_ssid", ssid);
        prefs.putString("wifi_pass", pass);

        request->send(200, "application/json", "{\"status\":\"restart\"}");
        delay(2000);
        ESP.restart();  // restartujemo da bi se povezo na novi WiFi
      }
    });
  server.begin();
}

// ----------------ISPIS I RAD--------------------
void buildMainText() {
  struct tm now;
  if (!getLocalTime(&now)) return;

  long danas = (now.tm_year + 1900) * 10000L + (now.tm_mon + 1) * 100 + now.tm_mday;

  struct tm sutraTm = now;
  sutraTm.tm_mday += 1;
  mktime(&sutraTm);
  long sutra = (sutraTm.tm_year + 1900) * 10000L + (sutraTm.tm_mon + 1) * 100 + sutraTm.tm_mday;

  String newText = "";
  int nowMin = now.tm_hour * 60 + now.tm_min;
  bool inClass = false;

  for (int i = 0; i < classCount; i++) {
    int start = timeToMinutes(classes[i].start);
    int end = timeToMinutes(classes[i].end);
    if (nowMin >= start && nowMin < end) {
      newText = String(classes[i].number) + ". cas | Kraj: " + classes[i].end + " | Jos " + String(end - nowMin) + " min";
      if (classes[i].dezurni != "") newText += " | Dezurni: " + classes[i].dezurni;
      inClass = true;
      break;
    }
  }

  if (!inClass) {
    if (classCount > 0 && nowMin < timeToMinutes(classes[0].start)) {
      newText = "PRIPREMA ZA NASTAVU";
    } else if (classCount > 0 && nowMin > timeToMinutes(classes[classCount - 1].end)) {
      newText = "KRAJ NASTAVE";
    } else {
      newText = "ODMOR";
    }
  }

  String obavijestiDio = "";

  for (int i = 0; i < notificationCount; i++) {
    long datumObavijesti = notifications[i].year * 10000L + notifications[i].month * 100 + notifications[i].day;

    if (datumObavijesti < danas) continue;

    if (datumObavijesti == danas || datumObavijesti == sutra) {
      String prefiks = (datumObavijesti == danas) ? " DANAS " : " SUTRA ";

      obavijestiDio += " |" + prefiks + notifications[i].text + " u ";

      if (notifications[i].hour < 10) obavijestiDio += "0";
      obavijestiDio += String(notifications[i].hour) + ":";

      if (notifications[i].minute < 10) obavijestiDio += "0";
      obavijestiDio += String(notifications[i].minute);
    }
  }
  newText += obavijestiDio;

  if (newText != lastText) {
    text = newText;
    lastText = newText;
    xPos = 128;
  }
}

void checkBell() {
  struct tm now;
  if (!getLocalTime(&now) || bellTestMode) return;
  int nowMin = now.tm_hour * 60 + now.tm_min;
  if (nowMin == lastBellMinute) return;

  for (int i = 0; i < classCount; i++) {
    if (nowMin == timeToMinutes(classes[i].start) || nowMin == timeToMinutes(classes[i].end)) {
      digitalWrite(RELAY_PIN, HIGH);
      delay(2000);
      digitalWrite(RELAY_PIN, LOW);
      lastBellMinute = nowMin;
      break;
    }
  }
}

// ---------------- SETUP I LOOP --------------------

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  configuration();

  strip.begin();             // inicijalizacija trake
  strip.show();              // ugasi sve LED diode na pocetku
  strip.setBrightness(100);  // podesi jacinu (0-255)

  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);

  prefs.begin("schoolbell", false);

  satR = prefs.getInt("satR", 255);
  satG = prefs.getInt("satG", 150);
  satB = prefs.getInt("satB", 0);
  textR = prefs.getInt("textR", 150);
  textG = prefs.getInt("textG", 255);
  textB = prefs.getInt("textB", 0);

  if (!LittleFS.begin()) {
    Serial.println("Greška pri mountovanju LittleFS");
    return;
  }
  // Pročitaj spaseni WiFi, ako ga nema koristi "lamija7" kao rezervu
  String savedSSID = prefs.getString("wifi_ssid", "Mesic");
  String savedPASS = prefs.getString("wifi_pass", "alma12345");

  if (savedSSID == "") {
    savedSSID = "Mesic";
    savedPASS = "alma12345";
  }else{
    savedSSID = prefs.getString("wifi_ssid", "Mesic");
    savedPASS = prefs.getString("wifi_pass", "alma12345");
  }
  WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi nije spojen - nastavljam bez WiFi");
    Serial.println(WiFi.status());
  }

  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
    String user = "";
    String pass = "";

    if (request->hasParam("username", true)) user = request->getParam("username", true)->value();
    if (request->hasParam("password", true)) pass = request->getParam("password", true)->value();

    Serial.println("Pokušaj prijave: " + user + " | " + pass);
    request->send(200, "text/plain", "Podaci primljeni!");
  });

  server.serveStatic("/", LittleFS, "/")
    .setDefaultFile("login.html")
    .setCacheControl("public, max-age=31536000");

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Fajl nije pronadjen");
  });

  server.begin();
  Serial.println("HTTP server started");

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0", 1);
  tzset();

  String r = prefs.getString("raspored", "");
  if (r != "") handleJson(r);
  String o = prefs.getString("obavijesti", "");
  if (o != "") handleJson(o);

  setupRoutes();
  startTime = millis();
}

void loop() {

  if (Serial.available()) {
    String podaciIzKabla = Serial.readStringUntil('\n');
    handleJson(podaciIzKabla);
  }

  display->fillScreen(0);

  if (startup) {
    display->setTextSize(2);
    display->setTextColor(display->color565(255, 150, 0));
    display->setCursor(45, 8);
    display->print("ETS");
    if (millis() - startTime > 3000) startup = false;
  } else if (bellTestMode == true) {
    display->setTextSize(1);
    display->setTextColor(display->color565(255, 255, 255));
    display->setCursor(2, 5);
    display->print("TESTIRANJE");
    display->setCursor(15, 18);
    display->print("ZVONA...");
    return;
  } else if (sosActive) {
    display->fillScreen(((millis() / 300) % 2) ? 0 : display->color565(255, 0, 0));
    if (millis() - sosBellTimer > sosPattern[sosStep]) {
      sosBellTimer = millis();
      digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
      sosStep = (sosStep + 1) % sosLen;
    }
    if (millis() - sosStartTime > 15000) {
      sosActive = false;
      digitalWrite(RELAY_PIN, LOW);
    }
  } else {

    display->setTextSize(2);
    display->setTextColor(display->color565(satR, satG, satB));  // boja sata
    display->setCursor(28, 0);
    display->print(getTimeString());

    display->setTextColor(display->color565(textR, textG, textB));  // boja scrolla
    display->setCursor(xPos, 17);
    display->print(text);

    xPos--;
    if (xPos < -((int)text.length() * 12)) xPos = 128;

    checkBell();
    buildMainText();
  }
  delay(30);
}