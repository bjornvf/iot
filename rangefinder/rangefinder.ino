#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

/*
ESP8266 connected to sonic sensor HC-SR04.
Trig -> D5
Echo -> D6
*/
const char *ID = "CLOSET_1";
const char *ssid = "ESP8266";
const char *password = "";
const int SPEED_OF_SOUND = 340; // micrometer pr microsec.
const int TRIG = D5;
const int ECHO = D6;

ESP8266WebServer server(80);

int readRange() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  int duration = pulseIn(ECHO, HIGH); //duration of the roundtrip sonic signal in microsecs
  return duration /2;
}

void handleRequest() {
  int sonicRange = readRange();
  int distance = 0;
  if (sonicRange != 0) {
    distance = sonicRange * SPEED_OF_SOUND; // distance in micrometers
  }
  Serial.println(String(sonicRange) + ", " + String(distance));
  server.send(200, "text/plain", "Range in time is " + String(sonicRange) + "microsec, range in distance is " + String(distance / 1000) + "mm.");
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("\nWIFI SSID is " + String(ssid));
  WiFi.softAP(ssid, password);
  server.on("/", handleRequest);
  server.begin();
  Serial.println("IP is " + String(WiFi.softAPIP()));
  digitalWrite(TRIG, LOW);
}

void loop() {
  server.handleClient();
}
