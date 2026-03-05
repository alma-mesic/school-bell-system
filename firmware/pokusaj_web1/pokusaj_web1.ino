#include <WiFi.h>
#include <ESPAsyncWebServer.h> 
#include <ArduinoJson.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Preferences.h>
#include "time.h"

// --- OBAVEZNE DEFINICIJE PIN-OVA ---
#define RELAY_PIN 18      
#define LED_STRIP_PIN 21  

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 2

MatrixPanel_I2S_DMA* display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
Preferences prefs;
AsyncWebServer server(80);

// --- GLOBALNE PROMJENLJIVE ---
String trenutniTekst = "Cekam raspored..."; 
bool emergencyMode = false;
bool startupOdrano = false;
unsigned long startupVrijeme;
int xPos = 128;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; 
const int   daylightOffset_sec = 3600; 

String getVrijeme() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){ return "--:--"; }
  char buf[6];
  strftime(buf, 6, "%H:%M", &timeinfo);
  return String(buf);
}

void configuration() {
  mxconfig.gpio.r1 = 25; mxconfig.gpio.g1 = 27; mxconfig.gpio.b1 = 26;
  mxconfig.gpio.r2 = 14; mxconfig.gpio.g2 = 13; mxconfig.gpio.b2 = 12;
  mxconfig.gpio.a = 23;  mxconfig.gpio.b = 19;  mxconfig.gpio.c = 5;
  mxconfig.gpio.d = 17;  mxconfig.gpio.clk = 16; mxconfig.gpio.lat = 4;
  mxconfig.gpio.oe = 15;
}

void setupRoutes() {
  // TEST ZVONA
  server.on("/api/test", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    if (doc["naredba"] == "TEST_START") {
      digitalWrite(RELAY_PIN, HIGH);
      trenutniTekst = "TESTIRANJE ZVONA";
    } else {
      digitalWrite(RELAY_PIN, LOW);
      trenutniTekst = "Sistem spreman";
    }
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  });

  // EMERGENCY
  server.on("/api/emergency", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    emergencyMode = (doc["naredba"] == "EMERGENCY_START");
    if (!emergencyMode) digitalWrite(RELAY_PIN, LOW);
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  });

  // RASPORED
  server.on("/api/raspored", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<4096> doc;
    deserializeJson(doc, data);
    trenutniTekst = doc["ispis"].as<String>();
    xPos = 128; 
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  digitalWrite(LED_STRIP_PIN, HIGH);

  configuration();
  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);

  WiFi.begin("59588d", "273370344");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  startupVrijeme = millis();
  setupRoutes();
}

void loop() {
  display->fillScreen(0);

  // 1. STARTUP (ETS na sredini 3 sekunde)
  if (!startupOdrano) {
    display->setTextSize(2);
    display->setTextColor(display->color565(0, 0, 255));
    display->setCursor(45, 8);
    display->print("ETS");
    if (millis() - startupVrijeme > 3000) startupOdrano = true;
    return; 
  }
  
  // 2. EMERGENCY (Blinkanje crveno)
  if (emergencyMode) {
    if ((millis() / 500 % 2)) display->fillScreen(display->color565(255, 0, 0));
    digitalWrite(RELAY_PIN, (millis() / 200 % 2)); 
    return;
  } 

  // 3. NORMALAN RAD (Dva reda)
  
  // --- GORNJI RED: SAT (Fiksni) ---
  display->setTextSize(1);
  display->setTextColor(display->color565(255, 255, 0)); // Žuti sat
  display->setCursor(49, 2); 
  display->print(getVrijeme());

  // --- DONJI RED: INFO (Scrolling) ---
  display->setTextColor(display->color565(0, 255, 0)); // Zeleni tekst
  display->setCursor(xPos, 20); 
  display->print(trenutniTekst);

  xPos--;
  // Reset skrola (Size 1 slovo je 6px široko)
  if (xPos < -((int)trenutniTekst.length() * 6)) {
    xPos = 128;
  }
  
  delay(30);
}