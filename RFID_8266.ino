#include <SPI.h>
#include "MFRC522.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "Myangle_2.4G" 
#define WIFI_PASSWORD "0819879414"

#define RST_PIN  D1
#define SS_PIN  D2

MFRC522 mfrc522(SS_PIN, RST_PIN);

String rfid_in = "";

void setup() {
  connectWifi();
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    rfid_in=rfid_read();
    Serial.println(">>>> " + rfid_in);
    delay(1000);
  }
  delay(1);
}

String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

void connectWifi() {
    Serial.begin(9600);
    Serial.println(WiFi.localIP());
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}