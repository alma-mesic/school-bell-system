#include <WiFi.h>
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "Mesic";
const char* password = "alma12345";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("Greška pri mountovanju LittleFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  // --- OBRADA LOGIN FORME (Primjer ako forma šalje POST na /login) ---
  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
    String user = "";
    String pass = "";
    
    if (request->hasParam("username", true)) user = request->getParam("username", true)->value();
    if (request->hasParam("password", true)) pass = request->getParam("password", true)->value();
    
    Serial.println("Pokušaj prijave: " + user + " | " + pass);
    request->send(200, "text/plain", "Podaci primljeni!");
  });

  // --- SERVIRANJE FAJLOVA (Slike, CSS, JS, HTML) ---
  server.serveStatic("/", LittleFS, "/")
        .setDefaultFile("login.html")
        .setCacheControl("public, max-age=31536000");

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Fajl nije pronadjen");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Loop ostaje prazan za asinkroni server
}