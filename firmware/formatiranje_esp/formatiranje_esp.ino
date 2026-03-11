#include "LittleFS.h"

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("Greška pri mountovanju LittleFS");
    return;
  }

  // Briše sve fajlove
  if (LittleFS.format()) {
    Serial.println("LittleFS je potpuno obrisan. Sada je prazan.");
  } else {
    Serial.println("Greška pri brisanju LittleFS.");
  }
}

void loop() {
  // Prazno, ništa više ne radi
}