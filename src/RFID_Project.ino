#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;      // IST = UTC+5:30 -> 5*3600 + 1800
const int daylightOffset_sec = 0;


#define SS_PIN 5
#define RST_PIN 21
#define LED_PIN 2

MFRC522 rfid(SS_PIN, RST_PIN);

// WiFi credentials
const char* ssid = "moto";
const char* password = "Ahkv@2005";

// Twilio credentials
const char* accountSid = "AC30a6b17d3857151180cdd93963c96b56";
const char* authToken = "d31d6bc08d596cb49eff386c0fea57b3";
const char* twilioNumber = "+17755226310";
const char* adminNumber = "+918150839959";

// Google Sheets webhook
const char* sheetsUrl = "https://script.google.com/macros/s/AKfycbx_LfUZPUz61uVMON41t1693O3231K8M25_09cxTr1yk3Bt8ufHwzDajjTeKR6lhTab/exec";

// Student database
struct Student {
  String uid;
  String name;
  String srn;
};

Student students[] = {
  {"33C9F28", "Ashik", "R23EI008"},
  {"93852EDA", "Piyush", "R23EI029"},
  {"23A5DC13", "Chandru Prasad", "R23EI013"}
};
const int numStudents = 3;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
Serial.println("Time config done");

}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
    uid.toUpperCase();
  }
  
  Serial.println("UID: " + uid);
  
  Student* matchedStudent = nullptr;
  for (int i = 0; i < numStudents; i++) {
    if (students[i].uid == uid) {
      matchedStudent = &students[i];
      break;
    }
  }
  
  if (matchedStudent != nullptr) {
    digitalWrite(LED_PIN, HIGH);
    
   struct tm timeinfo;
if (!getLocalTime(&timeinfo)) {
  Serial.println("Failed to get time");
  return;
}

char dateBuf[11];  // DD/MM/YYYY
char timeBuf[9];   // HH:MM:SS
strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", &timeinfo);
strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &timeinfo);

String date = String(dateBuf);
String time = String(timeBuf);

// now call your functions with real IST date/time
sendToSheets(uid, matchedStudent->name, matchedStudent->srn, date, time);
sendSMS(matchedStudent->name, matchedStudent->srn, date, time);

    
    delay(5000);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Unauthorized UID");
  }
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void sendToSheets(String uid, String name, String srn, String date, String time) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(sheetsUrl);
    http.addHeader("Content-Type", "application/json");
    
    String json = "{\"uid\":\"" + uid + "\",\"name\":\"" + name + "\",\"srn\":\"" + srn + "\",\"date\":\"" + date + "\",\"time\":\"" + time + "\"}";
    int httpCode = http.POST(json);
    
    if (httpCode > 0) {
      Serial.println("Sheets logged");
    }
    http.end();
  }
}

void sendSMS(String name, String srn, String date, String time) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(accountSid) + "/Messages.json";
    http.begin(url);
    
    // Basic Auth header (SID:Token base64 encoded)
    String auth = String(accountSid) + ":" + String(authToken);
    http.addHeader("Authorization", "Basic " + base64::encode(auth.c_str()));
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String body = "To=" + String(adminNumber) + 
                  "&From=" + String(twilioNumber) + 
                  "&Body=Student%20" + name + "%20(" + srn + ")%20marked%20Present%20on%20" + 
                  date + "%20at%20" + time;
    
    int httpCode = http.POST(body);
    Serial.println("SMS HTTP Code: " + String(httpCode));
    
    if (httpCode == 201) {
      Serial.println("SMS sent successfully");
    } else {
      Serial.println("SMS failed");
    }
    http.end();
  }
}

