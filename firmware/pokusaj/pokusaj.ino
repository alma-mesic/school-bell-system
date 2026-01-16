#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "time.h"

// ================== PANEL ==================
#define PANEL_RES_X 32
#define PANEL_RES_Y 16
#define PANEL_CHAIN 1

MatrixPanel_I2S_DMA *display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);

// ================== BELL ==================
#define RELAY_PIN 18   // PIN za relej (zvono)

// ================== GLOBAL ==================
bool startup = true;
unsigned long startTime;

int x = 32;
int16_t text_width;

bool bellOn = false;
unsigned long bellStartTime = 0;

// ================== TEXT ==================
String text = "Cekam raspored";

// ================== WIFI + TIME ==================
const char* ssid = "59588d";
const char* password = "273370344";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// ================== RASPORED STRUCT ==================
struct ClassTime {
  int number;
  String start;
  String end;
};

ClassTime classes[20];
int classCount = 0;

// ======== DODACI ZA NON-BLOCKING SOS I OBAVIJESTI ========
bool sosActive = false;
unsigned long sosStartTime = 0;
bool scheduleUpdated = false; // flag da je stigao novi raspored

String currentNotification = ""; // Tekst obavijesti koji se prikazuje
unsigned long notificationStartTime = 0;
const unsigned long notificationDisplayTime = 5000; // 5 sekundi

// Funkcija za pokretanje SOS treptanja
void startSOS() {
  sosActive = true;
  sosStartTime = millis();
}

// Funkcija za prikaz obavijesti
void showNotification(String notif) {
  currentNotification = notif;
  notificationStartTime = millis();
}

// ================== WIFI ==================
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Povezivanje na WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Povezano!");
}

// ================== TIME ==================
String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "00:00";

  char buffer[6];
  strftime(buffer, 6, "%H:%M", &timeinfo);
  return String(buffer);
}

// ================== CONFIGURATION ==================
void configuration(){
  mxconfig.gpio.r1 = 25; mxconfig.gpio.g1 = 27; mxconfig.gpio.b1 = 26;
  mxconfig.gpio.r2 = 14; mxconfig.gpio.g2 = 13; mxconfig.gpio.b2 = 12;
  mxconfig.gpio.a = 23; mxconfig.gpio.b = 19; mxconfig.gpio.c = 5;
  mxconfig.gpio.clk = 16; mxconfig.gpio.lat = 4; mxconfig.gpio.oe = 15;
}

// ================== DISPLAY FUNCTIONS ==================
void showStartupScreen() {
  display->fillScreen(0);
  display->setTextSize(1);
  display->setTextColor(display->color565(255, 0, 0));
  display->setCursor(6, 4);
  display->print("ETS");
}

// ================== TIME FUNCTION ==================
void showTime() {
  display->setTextSize(1);
  display->setTextColor(display->color565(0, 255, 255));
  display->setCursor(1, 0);
  display->print(getTimeString());
}

// ================== TEXT SCROLL FUNCTIONS ==================
void scrollText() {
  display->setTextColor(display->color565(255, 255, 0));
  display->setCursor(x, 8);
  display->print(text);

  x--;
  if (x < -text_width) x = 32;
}

// ================== MAIN FUNCTION ================== !!!!!!!!!!!!!!!!!
void drawMainScreen() {
  display->fillScreen(0);
  showTime();
  scrollText();
}

// ================== JSON HANDLER ==================
void handleJson(String json) {
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.println("JSON Error!");
    return;
  }

  String tip = doc["tip"] | "";

  if (tip == "raspored") {
    // ----- UCITAVANJE RASPOREDA -----
    classCount = 0;
    JsonArray casovi = doc["casovi"].as<JsonArray>();
    for (JsonObject cls : casovi) {
      classes[classCount].number = cls["cas"];
      classes[classCount].start  = cls["pocetak"].as<String>();
      classes[classCount].end    = cls["kraj"].as<String>();
      classCount++;
    }
    text = "Raspored ucitan";
    text_width = text.length() * 6;
    scheduleUpdated = true; // signaliziramo da je stigao novi raspored
  }
  else if (tip == "zvono") {
    String akcija = doc["akcija"] | "";
    if (akcija == "start") {
      digitalWrite(RELAY_PIN, HIGH);
      bellOn = true;
      bellStartTime = millis();
      text = "Zvono!";
      text_width = text.length() * 6;
    }
    else if (akcija == "stop") {
      digitalWrite(RELAY_PIN, LOW);
      bellOn = false;
      text = "Zvono zaustavljeno";
      text_width = text.length() * 6;
    }
  }
  else if (tip == "emergency") {
    String stanje = doc["stanje"] | "";
    if (stanje == "ukljuceno") {
      startSOS(); // non-blocking SOS
      text = "SOS";
      text_width = text.length() * 6;
    }
  }
  else if (tip == "wifi") {
    // Promjena WiFi-a (trenutno samo za demonstraciju, reconnect ručno)
    String newSSID = doc["ssid"] | "";
    String newPass = doc["password"] | "";
    if (newSSID != "" && newPass != "") {
      text = "Promjena WiFi";
      text_width = text.length() * 6;
      Serial.print("Novi WiFi: "); Serial.println(newSSID);
      Serial.print("Password: "); Serial.println(newPass);
    }
  }
  else if (tip == "obavijesti") {
    JsonArray lista = doc["lista"].as<JsonArray>();
    if (lista.size() > 0) {
      showNotification(lista[0]["naziv"].as<String>()); // non-blocking prikaz
    }
  }
  else {
    Serial.println("Nepoznat tip JSON-a");
  }
}

// ================== TIME CHECK ==================
void checkSchedule() {
  String now = getTimeString();

  for (int i = 0; i < classCount; i++) {
    if (now == classes[i].start) {
      text = "Pocetak casa";
      digitalWrite(RELAY_PIN, HIGH);
      bellOn = true;
      bellStartTime = millis();
    }

    if (now == classes[i].end) {
      text = "Odmor";
      digitalWrite(RELAY_PIN, HIGH);
      bellOn = true;
      bellStartTime = millis();
    }
  }

  if (bellOn && millis() - bellStartTime > 3000) {
    digitalWrite(RELAY_PIN, LOW);
    bellOn = false;
  }

  text_width = text.length() * 6;
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // --- PIN KONFIGURACIJA MATRICE ---
  configuration();

  // --- MATRIX SETTINGS ---
  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);
  display->clearScreen();
  display->setTextWrap(false);

  startTime = millis();

  // --- WIFI + NTP ---
  connectWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Cekam NTP...");
    delay(500);
  }

  text_width = text.length() * 6;
}

// ================== LOOP ==================
void loop() {

  // ----- JSON PRIJEM (SERIAL) -----
  if (Serial.available()) {
    String json = Serial.readStringUntil('\n');
    handleJson(json);
  }

  // ----- STARTUP -----
  if (startup) {
    showStartupScreen();

    if (millis() - startTime > 10000) {
      startup = false;
    }

    delay(100);
    return;
  }

  // ----- SOS TREPTANJE -----
  if (sosActive) {
    if (((millis() - sosStartTime) / 300) % 2 == 0) display->fillScreen(display->color565(255,0,0));
    else display->fillScreen(0);

    if (millis() - sosStartTime > 1800) sosActive = false; // 6 * 300ms
  }
  else {
    // ----- PRIKAZ OBAVIJESTI -----
    if (currentNotification != "" && millis() - notificationStartTime < notificationDisplayTime) {
      text = currentNotification;
      text_width = text.length() * 6;
    }
    else if (scheduleUpdated) {
      text = "Raspored ucitan"; // prikazi poruku da je stigao
      text_width = text.length() * 6;
      scheduleUpdated = false; // resetiramo flag
    }
    else {
      currentNotification = ""; // zavrsila obavijest
      checkSchedule(); // provjera rasporeda i casa
    }

    drawMainScreen();
  }

  delay(100);
}
