#include <WiFi.h>
#include <WebServer.h>
#include "LittleFS.h"

const char* ssid = "59588d";
const char* password = "273370344";

WebServer server(80);

void handleFile(String path) {
  // LittleFS zahtijeva da putanja počinje sa /
  if (!path.startsWith("/")) path = "/" + path;

  // Ako se traži samo "/", preusmjeri na login.html
  if (path == "/") path = "/login.html";

  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    String contentType = "text/plain";

    if (path.endsWith(".html")) contentType = "text/html";
    else if (path.endsWith(".css")) contentType = "text/css";
    else if (path.endsWith(".js")) contentType = "application/javascript";
    else if (path.endsWith(".png")) contentType = "image/png";
    else if (path.endsWith(".jpg") || path.endsWith(".jpeg")) contentType = "image/jpeg";
    else if (path.endsWith(".ico")) contentType = "image/x-icon";

    server.streamFile(file, contentType);
    file.close();
  } else {
    Serial.println("Fajl nije pronadjen: " + path);
    server.send(404, "text/plain", "Fajl nije pronadjen: " + path);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin()) {
    Serial.println("Greška pri mountovanju LittleFS");
    return;
  }

  // Glavna ruta
  server.on("/", []() { handleFile("/login.html"); });

  server.onNotFound([]() {
    handleFile(server.uri());
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
