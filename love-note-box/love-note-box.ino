
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "display_selection.h"
#include "internet_connection.h"

String previousMessage = "This is the startup message!!!";

void displayMessage(const char* message)
{
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(message, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(message);
  }
  while (display.nextPage());
}

void setup()
{
  //display.init(115200);
  display.init(115200, true, 2, false);
  displayMessage(previousMessage.c_str());
  display.hibernate();
  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(endpoint)) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);

          // Parse JSON
          DynamicJsonDocument doc(1024);
          DeserializationError error = deserializeJson(doc, payload);
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }

          // Look for the key "message"
          String message = doc["message"];
          
          // Update display only if the message is different from the previous one
          if (message != previousMessage) {
            displayMessage(message.c_str());
            Serial.print("Message: ");
            Serial.println(message);
            previousMessage = message;
          }
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  delay(30000); // Wait 30 seconds before sending the next request
}
