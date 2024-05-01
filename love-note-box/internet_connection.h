#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "LaceyWireless";
const char* password = "7402437306";
const char* endpoint = "https://holoapi-production.up.railway.app/love-note"; 


void connectToWiFi() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println(WiFi.localIP());
}
