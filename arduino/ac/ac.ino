#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define USE_SERIAL Serial


String user = "tei";
//String user = "ueno";
//String user = "meguro";

void setup() {
  USE_SERIAL.begin(115200);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }

  if(user=="tei") {
//    wifiMulti.addAP("しゃろの iPhone XS", "avengersJ8");
    wifiMulti.addAP("Buffalo-G-89C0", "wmprhbk75icdn"); // meguro wifi
  }
  if(user=="ueno") {
    wifiMulti.addAP("Buffalo-G-8B00", "yidvvu8hccrcw");// ueno wifi
  }
  if(user=="meguro") {
    wifiMulti.addAP("Buffalo-G-89C0", "wmprhbk75icdn"); // meguro wifi
  }
}

void loop() {
  String payload;
  if((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
    http.begin("http://164.70.117.162:3002/api/v1/ac_state/tei"); //HTTP
    
    USE_SERIAL.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            payload = http.getString();
            USE_SERIAL.println(payload);
        }
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
     http.end();
  }

  Serial.print("[payload] ");
  Serial.println(payload);

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();

  int state = obj["ac_state"];
  Serial.print("[state] ");
  USE_SERIAL.println(state);
  pinMode(2, OUTPUT);
  if(state==1){
    digitalWrite(2, HIGH);
  }else {
    digitalWrite(2, LOW);
  }
  delay(1000);
}
