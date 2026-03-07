#include <WiFi.h>
#include <AsyncTCP.h>
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
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) request->send(200);
    else request->send(404);
  });

  // --- POPRAVLJENA RUTA ZA OBAVJEŠTENJA ---
  server.on("/poshalji", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<2048> doc;
    deserializeJson(doc, data);
    
    if (doc.containsKey("vrijednost")) {
      JsonArray obavijesti = doc["vrijednost"];
      String noviTekst = "";
      for (JsonObject v : obavijesti) {
        String ime = v["ime"].as<String>();
        String vrijeme = v["vrijeme"].as<String>();
        noviTekst += ime + " u " + vrijeme + "  |"; 
      }
      if (noviTekst != "") {
        trenutniTekst = noviTekst;
        xPos = 128;
        emergencyMode = false; // Isključi emergency ako stigne nova vijest
      }
    }
  });

  // --- POPRAVLJENA RUTA ZA EMERGENCY (Blinkanje) ---
  server.on("/api/emergency", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    String naredba = doc["naredba"].as<String>();
    emergencyMode = (naredba == "EMERGENCY_START");
    Serial.println(emergencyMode ? "EMERGENCY START" : "EMERGENCY STOP");
  });

  // --- RUTA ZA RASPORED ---
  server.on("/api/raspored", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<4096> doc;
    deserializeJson(doc, data);
    if (doc.containsKey("ispis")) {
        trenutniTekst = doc["ispis"].as<String>();
        xPos = 128;
    }
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Daj mu sekundu da se "probudi"
  Serial.println("Sistem se pokrece..."); // Ako ovo vidis, Serial radi!
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  digitalWrite(LED_STRIP_PIN, HIGH);

  configuration();
  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);

  WiFi.begin("lamija7", "112345678");
  Serial.print("Povezujem se na WiFi");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    display->fillScreen(0);
    display->setCursor(0, 8);
    display->print("Spajanje...");
    }

  Serial.println("\nPovezan!");
  Serial.print("IP adresa: ");
  Serial.println(WiFi.localIP());

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
  display->setTextSize(2);
  display->setTextColor(display->color565(255, 150, 0)); // Žuti sat
  display->setCursor(28, 0); 
  display->print(getVrijeme());

  // --- DONJI RED: INFO (Scrolling) ---
  display->setTextColor(display->color565(0, 255, 0)); // Zeleni tekst
  display->setCursor(xPos, 17); 
  display->print(trenutniTekst);

  xPos--;
  // Reset skrola (Size 1 slovo je 6px široko)
  if (xPos < -((int)trenutniTekst.length() * 6)) {
    xPos = 128;
  }
  
  delay(30);
}