#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

/*
  ESP8266 connected to IR motion sensor HC-SR501.
  Out -> D4
*/

const char *ssid = "ESP8266";
const char *password = "";
const int MOTION = D4;

ESP8266WebServer server(80);

boolean motionState = false;
unsigned long lastTimestamp = 0;

boolean hasMotion() {
  return digitalRead(MOTION) == HIGH;
}

String formatTimestamp(unsigned long timestamp) {
  unsigned long secks = timestamp / 1000;
  unsigned long mins = secks / 60;
  unsigned long hours = mins / 60;
  return String(hours) + ":" + mins % 60 + ":" + secks % 60 + "." + timestamp % 1000;
}

void checkMotion() {
  boolean newMotionState = hasMotion();
  if (motionState != newMotionState) {
    motionState = newMotionState;
    unsigned long timestamp = millis();
    unsigned long duration = timestamp - lastTimestamp;
    lastTimestamp = timestamp;
    
    if (motionState) {
      Serial.println(formatTimestamp(timestamp) + " Motion detected (" + duration + ")");
    } else {
      Serial.println(formatTimestamp(timestamp) + " No Motion detected (" + duration + ")");
    }
  }
}

void handleRequest() {
  server.send(200, "text/plain", "Motion: " + String(motionState));
}

void setup() {
  Serial.begin(9600);
  pinMode(MOTION, INPUT);
  Serial.println("\nWIFI SSID is " + String(ssid));
  WiFi.softAP(ssid, password);
  server.on("/", handleRequest);
  server.begin();
  Serial.println("IP is " + String(WiFi.softAPIP()));
}

void loop() {
  checkMotion();
  server.handleClient();
}
