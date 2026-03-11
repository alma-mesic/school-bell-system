#include <WiFi.h>
#include <WebServer.h>
#include "LittleFS.h"

const char* ssid = "59588d";
const char* password = "273370344";

WebServer server(80);

void handleFile(String path){
  if(LittleFS.exists(path)){
    File file = LittleFS.open(path, "r");
    String contentType = "text/plain";

    if(path.endsWith(".html")) contentType = "text/html";
    else if(path.endsWith(".css")) contentType = "text/css";
    else if(path.endsWith(".js")) contentType = "application/javascript";
    else if(path.endsWith(".png")) contentType = "image/png";
    else if(path.endsWith(".jpg")) contentType = "image/jpeg";

    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(!LittleFS.begin()) {
  Serial.println("An error has occurred while mounting LittleFS");
  return;
  }
  Serial.println("LittleFS mounted successfully");


  server.on("/", [](){ handleFile("/login.html"); });
  server.on("/index.html", [](){ handleFile("/index.html"); });
  server.on("/style.css", [](){ handleFile("/style.css"); });
  server.on("/script1.js", [](){ handleFile("/script1.js"); });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

}
