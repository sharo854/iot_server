#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

WiFiMulti wifiMulti;
int inPin = 17;
int val = 0;
int preval = val;

//void IRAM_ATTR stateChange(){
//  val = digitalRead(inPin);
//  Serial.println(val);
//  digitalWrite(2, val);
//  preval = val;
//  if((wifiMulti.run() == WL_CONNECTED)) {
//    HTTPClient http;
//    http.begin("http://164.70.117.162:3002/change/state_now/tei/"+String(val));
//    http.POST("");
//    http.end();
//  }
//}

void setup() {
//  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(inPin, INPUT_PULLUP);
//  attachInterrupt(inPin, stateChange, CHANGE);

//  wifiMulti.addAP("しゃろの iPhone XS", "avengersJ8");
//  wifiMulti.addAP("Buffalo-G-8B00", "yidvvu8hccrcw");// ueno wifi
  wifiMulti.addAP("Buffalo-G-89C0", "wmprhbk75icdn"); // meguro wifi
}


void loop() {
  val = digitalRead(inPin);
  if(preval!=val) {
    digitalWrite(2, val);
    preval = val;
    if((wifiMulti.run() == WL_CONNECTED)) {
      HTTPClient http;
      http.begin("http://164.70.117.162:3002/change/state_now/tei/"+String(val));
      http.POST("");
      http.end();
    }
  }
  delay(1000);
}
