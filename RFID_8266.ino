#include <SPI.h>
#include "MFRC522.h"
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define RST_PIN D1
#define SS_PIN D2
#define WIFI_SSID "joten" 
#define WIFI_PASSWORD "0871108067"
#define FIREBASE_HOST "rfid-database-8c0f2-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_KEY "2nyECFKpBSthELM3cbV6rjxEQodeJSuaTQviU2iF"

FirebaseData firebaseData;
bool state = false;
MFRC522 mfrc522(SS_PIN, RST_PIN);
String rfid_in = "";


void setup() {
  Serial.begin(9600);
  connectWifi();
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    rfid_in = rfid_read();
    Serial.println("===> " + rfid_in);
    sendToFirebase();
    delay(1000);
  }
  delay(1);
}

String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

//ConnectWIFI

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
//SaveonFirebase
void sendToFirebase() {
 delay(1000);

  // Create a unique key for the new node
  String newNodeKey = "/RFID/USER/" + String(millis());

  // Check if the same RFID data already exists in Firebase
  String existingNodeKey = findExistingNode(rfid_in);

  if (!existingNodeKey.isEmpty()) {
    // Update the existing node
    newNodeKey = existingNodeKey;
  }

  FirebaseJson json;
  json.add("rfidData", rfid_in);
  json.add("timestamp", String(millis()));  // Add a timestamp

  if (Firebase.setJSON(firebaseData, newNodeKey, json)) {
    Serial.println("Data added to Firebase");
  } else {
    Serial.println("Error : " + firebaseData.errorReason());
  }
}

String findExistingNode(const String &rfidData) {
  String existingKey = "";
  return existingKey;
}

  


