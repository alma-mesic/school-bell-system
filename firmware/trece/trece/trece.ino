#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "time.h"
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include "LittleFS.h"

#include <Fonts/Org_01.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>

// ---------------- KONFIGURACIJA ---------------------
#define RELAY_PIN 18
#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 2

#define LED_PIN 32     // broj pina led trake
#define LED_COUNT 200  // broj ledica na traci
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int ledMode = 0;
#define LED_MODE_OFF 0
#define LED_MODE_COLOR 1
#define LED_MODE_RAINBOW 2
#define LED_MODE_THEATER 3
#define LED_MODE_THEATER_RAINBOW 4

MatrixPanel_I2S_DMA *display;
HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);
Preferences prefs;
AsyncWebServer server(80);

// ---------------- GLOBALNE PROMJENLJIVE --------------------
String fontTekst = "org-01";
String fontSat = "org-01";

bool startup = true;
unsigned long startTime;
int xPos = 128;

bool bellTestMode = false;
int lastBellMinute = -1;
String text = "Cekam raspored...";
String lastText = "";

bool bellActive = false;
unsigned long bellStart = 0;
int lastMinuteChecked = -1;

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

int ledR = 255, ledG = 255, ledB = 255;  // Boja LED trake
int satR = 255, satG = 0, satB = 0;      // Boja sata na matrici
int textR = 0, textG = 255, textB = 0;   // Boja scroll teksta


// ---------------- POMOĆNE FUNKCIJE --------------------

void configuration() {
  mxconfig.gpio.r1 = 25;
  mxconfig.gpio.g1 = 26;//27 zamjenjeno zbog problema prikaza boje (kod racuna RBG umjesto RGB)
  mxconfig.gpio.b1 = 27;//26
  mxconfig.gpio.r2 = 14;
  mxconfig.gpio.g2 = 12;//13
  mxconfig.gpio.b2 = 13;//12
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
  if ((millis() / 500) % 2 == 0) {
    strftime(buf, 6, "%H:%M", &t);
  } else {
    strftime(buf, 6, "%H %M", &t);
  }
  return String(buf);
}

int timeToMinutes(const String &t) {
  int separatorIndex = t.indexOf(':');
  if (separatorIndex == -1) return 0;

  int sati = t.substring(0, separatorIndex).toInt();
  int minuti = t.substring(separatorIndex + 1).toInt();

  return sati * 60 + minuti;
}

// ---------------- WEB SERVER RUTE (ZA WEB APP) --------------------

void handleJson(String json) {
  StaticJsonDocument<4096> doc;
  if (deserializeJson(doc, json)) return;

  String tip = doc["tip"] | "";
  String mode = doc["mode"];

  if (tip == "raspored") {
    classCount = 0;
    for (JsonObject c : doc["casovi"].as<JsonArray>()) {
      if (classCount < 20) {
        int broj = c["cas"] | c["redniBroj"] | 0;
        classes[classCount++] = { c["dan"] | "", c["cas"].as<int>() | 0, c["pocetak"] | "", c["kraj"] | "", c["dezurni"] | "" };
      }
    }
    prefs.putString("raspored", json);
  } 
  else if (tip == "obavijesti") {
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
  } 
  else if (tip == "emergency") {
    bool stanje = doc["stanje"] | false;
    if (stanje) {
      sosActive = true;
      sosStartTime = millis();
      sosStep = 0;
    } else {
      sosActive = false;
      digitalWrite(RELAY_PIN, LOW);
    }
  } 
  else if (tip == "zvono") {
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
  else if (tip == "emergency_stop") {
    sosActive = false;
    digitalWrite(RELAY_PIN, LOW);
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
        ledMode = LED_MODE_COLOR;

        prefs.putInt("ledMode", ledMode);
        prefs.putInt("ledR", r);
        prefs.putInt("ledG", g);
        prefs.putInt("ledB", b);

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
    }
    else if (naredba == "LED_MODE") {
      String mode = doc["mode"];
      if (mode == "rainbow") {
        ledMode = LED_MODE_RAINBOW;
      }
      else if (mode == "theaterChase") {
        ledMode = LED_MODE_THEATER;
      }
      else if (mode == "theaterChaseRainbow") {
        ledMode = LED_MODE_THEATER_RAINBOW;
      }
      else if (mode == "off") {
        ledMode = LED_MODE_OFF;
      }
      prefs.putInt("ledMode", ledMode); // spremanje u memoriju
    }
    else if (naredba == "SET_FONTS") {
      fontTekst = doc["font_tekst"].as<String>();
      fontSat = doc["font_sat"].as<String>();

      prefs.putString("fontTekst", fontTekst);
      prefs.putString("fontSat", fontSat);

      Serial.println("Font tekst: " + fontTekst);
      Serial.println("Font sat: " + fontSat);
    }
    /*else if (naredba == "LED_CONTROL") { ovo je za gasenje trake (provjeriti da li je potrebno)
      String stanje = doc["stanje"];
      if (stanje == "ON") {
        ledMode = LED_MODE_COLOR;
      } else {
        ledMode = LED_MODE_OFF;
      }
      prefs.putInt("ledMode", ledMode);
    }*/
    request->send(200, "application/json", "{\"status\":\"ok\"}");
   
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
          // provjera stare šifre
          if (doc["stara"] != prefs.getString("adminPass", "admin")) {
            request->send(403, "application/json", "{\"status\":\"fail\"}");
            return;
          }

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

    server.on("/api/get_user", HTTP_GET, [](AsyncWebServerRequest *request) {
      String user = prefs.getString("adminUser", "admin");
      request->send(200, "text/plain", user);
    });
    //server.begin();
}

// ----------------ISPIS I RAD--------------------
void buildMainText() {
    struct tm now;
    if (!getLocalTime(&now)) return;

    long danas = (now.tm_year + 1900) * 10000L + (now.tm_mon + 1) * 100 + now.tm_mday;
    int currentTotalMinutes = now.tm_hour * 60 + now.tm_min;

    struct tm sutraTm = now;
    sutraTm.tm_mday += 1;
    mktime(&sutraTm);
    long sutra = (sutraTm.tm_year + 1900) * 10000L + (sutraTm.tm_mon + 1) * 100 + sutraTm.tm_mday;

    String newText = "";
    int nowMin = currentTotalMinutes;
    bool inClass = false;

    for (int i = 0; i < classCount; i++) {
      int start = timeToMinutes(classes[i].start);
      int end = timeToMinutes(classes[i].end);
      if (nowMin >= start && nowMin < end) {
        newText = String(classes[i].number) + ". cas | Kraj casa u: " + classes[i].end + " | Jos " + String(end - nowMin) + " min";
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
      int notifTotalMinutes = notifications[i].hour * 60 + notifications[i].minute;

      if (datumObavijesti < danas) continue;
      if (datumObavijesti == danas) {
        if (currentTotalMinutes > (notifTotalMinutes + 1)) continue;
      }

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
    if (sosActive) return;

    struct tm now;
    if (!getLocalTime(&now) || bellTestMode) return;
    int nowMin = now.tm_hour * 60 + now.tm_min;
    if (nowMin == lastBellMinute) return;

    for (int i = 0; i < classCount; i++) {
      if (nowMin == timeToMinutes(classes[i].start) || nowMin == timeToMinutes(classes[i].end)) {
        digitalWrite(RELAY_PIN, HIGH);
        bellActive = true;
        bellStart = millis();
        lastBellMinute = nowMin;
        break;
      }
    }
}

void removeExpiredNotifications() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return;

    now = mktime(&timeinfo);

    int j = 0;
    for (int i = 0; i < notificationCount; i++) {
      struct tm notifTime;
      notifTime.tm_year = notifications[i].year - 1900;
      notifTime.tm_mon = notifications[i].month - 1;
      notifTime.tm_mday = notifications[i].day;
      notifTime.tm_hour = notifications[i].hour;
      notifTime.tm_min = notifications[i].minute;
      notifTime.tm_sec = 0;
      time_t notifEpoch = mktime(&notifTime);

      if (difftime(notifEpoch, now) > 0) {
        // Notifikacija još traje, čuvamo je
        notifications[j++] = notifications[i];
      }
    }
    notificationCount = j;  // update count
}

void promijeniUsername(String noviUsername) {
    prefs.putString("username", noviUsername);
}

void promijeniSifru(String novaSifra) {
    prefs.putString("password", novaSifra);
}

String ucitajUsername() {
    return prefs.getString("username", "defaultUser");
}

String ucitajSifru() {
    return prefs.getString("password", "1234");
}


// --------------------- BOJE ----------------------
void runLedMode() {
  static uint16_t hue = 0;
  static int step = 0;

  if (ledMode == LED_MODE_OFF) {
    strip.clear();
    strip.show();
    return;
  }
  else if (ledMode == LED_MODE_COLOR) {
    strip.show();
  }
  else if (ledMode == LED_MODE_RAINBOW) {
    strip.rainbow(hue);
    strip.show();
    hue += 256;
  }
  else if (ledMode == LED_MODE_THEATER) {
    strip.clear();
    for (int i = step; i < strip.numPixels(); i += 3) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    step++;
    if (step >= 3) step = 0;
  }

  else if (ledMode == LED_MODE_THEATER_RAINBOW) {
    strip.clear();
    for (int i = step; i < strip.numPixels(); i += 3) {
      uint16_t h = hue + i * 65536L / strip.numPixels();
      uint32_t c = strip.gamma32(strip.ColorHSV(h));
      strip.setPixelColor(i, c);
    }
    strip.show();
    step++;
    if (step >= 3) step = 0;
    hue += 256;
  }
}

void colorWipe(uint32_t color, int wait) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
      strip.show();
      delay(wait);
    }
}

// -------------------- FONTOVI --------------------
int x,y,size;
void setFontSat() {
  if(fontSat=="org_01"){
    display->setFont(&Org_01);
    x=45;
    y=12;
    size=2;
  }
  else if (fontSat == "mono") {
    display->setFont(&FreeMono9pt7b);
    x=37;
    y=12;
    size=1;
  }
  else if (fontSat == "mono-bold") {
    display->setFont(&FreeMonoBold9pt7b);
    x=37;
    y=12;
    size=1;
  }
  else if(fontSat == "mono-bold-oblique"){
    display->setFont(&FreeMonoBoldOblique9pt7b); 
    x=37;
    y=12;
    size=1;
  }
  else if(fontSat == "sans"){
    display->setFont(&FreeSansBold9pt7b); 
    x=50;
    y=14;
    size=1;
  }
  else if(fontSat == "serif"){
    display->setFont(&FreeSerifBold9pt7b); 
    x=50;
    y=14;
    size=1;
  }
}
int x1,y2,size1;
void setFontText() {
  if(fontTekst=="org_01"){
    display->setFont(&Org_01);
    x1=xPos;
    y2=28;
    size=2;
  }
  else if (fontTekst == "mono") {
    display->setFont(&FreeMono9pt7b);
    x1=xPos;
    y2=28;
    size1=1;
  }
  else if (fontTekst == "mono-bold") {
    display->setFont(&FreeMonoBold9pt7b);
    x1=xPos;
    y2=28;
    size1=1;
  }
  else if (fontTekst == "mono-bold-oblique") {
    display->setFont(&FreeMonoBoldOblique9pt7b); 
    x1=xPos;
    y2=28;
    size1=1;
  }
  else if (fontTekst == "sans") {
    display->setFont(&FreeSansBold9pt7b); 
    x1=xPos;
    y2=28;
    size1=1;
  }
  else if (fontTekst == "serif") {
    display->setFont(&FreeSerifBold9pt7b); 
    x1=xPos;
    y2=28;
    size1=1;
  }

}
// ---------------- SETUP I LOOP --------------------

void setup() {
  Serial.begin(115200);

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pinMode(RELAY_PIN, OUTPUT);
  configuration();

  strip.begin();             // inicijalizacija trake
  strip.show();              // ugasi sve LED diode na pocetku
  strip.setBrightness(100);  // podesi jacinu (0-255)

  display = new MatrixPanel_I2S_DMA(mxconfig);
  display->begin();
  display->setBrightness8(120);

  prefs.begin("schoolbell", false);
  //prefs.begin("user-data", false);

  fontTekst = prefs.getString("fontTekst", "org_01");
  fontSat = prefs.getString("fontSat", "org_01");

  satR = prefs.getInt("satR", 255);
  satG = prefs.getInt("satG", 150);
  satB = prefs.getInt("satB", 0);
  textR = prefs.getInt("textR", 150);
  textG = prefs.getInt("textG", 255);
  textB = prefs.getInt("textB", 0);

  ledMode = prefs.getInt("ledMode", LED_MODE_OFF);
  ledR = prefs.getInt("ledR", 255);
  ledG = prefs.getInt("ledG", 255);
  ledB = prefs.getInt("ledB", 255);

  if (ledMode == LED_MODE_COLOR) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(ledR, ledG, ledB));
    }
    strip.show();
  }

  if (!LittleFS.begin()) {
    Serial.println("FS fail");

    LittleFS.format();

    if (!LittleFS.begin()) {
      Serial.println("FS fatal");
    }
  }
  // Pročitaj spaseni WiFi, ako ga nema koristi "lamija7" kao rezervu
  String savedSSID = prefs.getString("wifi_ssid", "59588d");
  String savedPASS = prefs.getString("wifi_pass", "273370344");

  WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

  int pokusaji = 0;
  while (WiFi.status() != WL_CONNECTED && pokusaji < 20) {
    delay(500);
    Serial.print(".");
    pokusaji++;
  }

  //Ako se desi greška, ESP32 prestaje biti "klijent" i postaje "ruter".
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi fail → AP mode");

    WiFi.softAP("SchoolBell", "12345678");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("\nPovezan na WiFi!");
    Serial.print("IP adresa: ");
    Serial.println(WiFi.localIP());
  }

  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("username", true) || !request->hasParam("password", true)) {
      request->send(400, "text/plain", "Fale podaci");
      return;
    }

    String user = request->getParam("username", true)->value();
    String pass = request->getParam("password", true)->value();

    String savedUser = prefs.getString("adminUser", "admin");
    String savedPass = prefs.getString("adminPass", "admin");

    if (user == savedUser && pass == savedPass) {
      request->send(200, "text/plain", "OK");
    } else {
      request->send(401, "text/plain", "FAIL");
    }

    /*if (request->hasParam("username", true)) user = request->getParam("username", true)->value();
  if (request->hasParam("password", true)) pass = request->getParam("password", true)->value();
  
  Serial.println("Pokušaj prijave: " + user + " | " + pass);
  request->send(200, "text/plain", "Podaci primljeni!");*/
  });
  server.serveStatic("/", LittleFS, "/")
    .setDefaultFile("login.html")
    .setCacheControl("public, max-age=31536000");

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0", 1);
  tzset();

  String r = prefs.getString("raspored", "");
  if (r != "") handleJson(r);
  String o = prefs.getString("obavijesti", "");
  if (o != "") handleJson(o);

  setupRoutes();
  startTime = millis();

  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
    if (Serial.available()) {
      String podaciIzKabla = Serial.readStringUntil('\n');
      handleJson(podaciIzKabla);
    }

    display->fillScreen(0);

    if (startup) {
      display->setTextSize(size);
      display->setTextColor(display->color565(255, 150, 0));
      display->setCursor(x, y);
      display->print("ETS");
      if (millis() - startTime > 3000) startup = false;
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
      setFontSat();
      display->setTextSize(size);
      display->setTextColor(display->color565(satR, satG, satB));  // boja sata
      display->setCursor(x, y);
      display->print(getTimeString());

      if (bellTestMode) {
        display->setTextSize(size);
        display->setTextColor(display->color565(255, 255, 255));
        display->setCursor(x1, y2);
        display->print("--- TEST ZVONA ---");
      } else {
        setFontText();
        display->setTextSize(size);
        display->setTextColor(display->color565(textR, textG, textB));
        display->setCursor(x1, y2);
        display->print(text);
      }

      xPos--;

      int textWidth = text.length() * 10; //(*12) *(size1*6)
      if (xPos < -textWidth) {
        xPos = 128;
      }

      //if (xPos < -((int)text.length() * 12)) xPos = 128;

      //checkBell();
      struct tm now;
      if (getLocalTime(&now)) {
        if (now.tm_min != lastMinuteChecked) {
          lastMinuteChecked = now.tm_min;
          checkBell();
        }
      }

      if (bellActive && millis() - bellStart > 2000) {
        digitalWrite(RELAY_PIN, LOW);
        bellActive = false;
      }
      buildMainText();
    }

    static unsigned long lastBuildTime = 0;
    if (millis() - lastBuildTime > 2000) {  // Ažuriraj tekst svake 2 sekunde
      buildMainText();
      lastBuildTime = millis();
    }

    runLedMode();
    delay(20);
}