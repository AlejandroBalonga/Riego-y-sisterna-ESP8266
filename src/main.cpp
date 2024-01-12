#include <Arduino.h>
#include "PCF8574.h"
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#define LED D4
#define TRIAC D5
#define IN_A D6
#define IN_B D7

// Set i2c address
PCF8574 pcf8574(0x20);

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, false);
  pinMode(TRIAC, OUTPUT);
  digitalWrite(TRIAC, true);
  pinMode(IN_A, INPUT_PULLUP);
  pinMode(IN_B, INPUT_PULLUP);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Config Riego WIFI");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(TRIAC, true); // apago la bomba
    digitalWrite(LED, !digitalRead(LED));
    WiFiManager wifiManager;
    wifiManager.autoConnect("Config Riego WIFI");
  }
  digitalWrite(LED, digitalRead(IN_A));
  digitalWrite(TRIAC, digitalRead(IN_B));
}