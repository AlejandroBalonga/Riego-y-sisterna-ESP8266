#include <Arduino.h>
#include "PCF8574.h"
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <LittleFS.h>
#include <GitHubOTA.h>
#include <GitHubFsOTA.h>

// This string should correspond to github tag used for Releasing (via. Github Actions)
// https://registry.platformio.org/libraries/axcap/Esp-GitHub-OTA
#define VERSION "0.0.4"
/*despues de cambiar esta version crear un nuevo TAG como v0.0.2
 con la version actual y realizar el commit y push (git push origin v0.0.2) */

// Replace your_username/your_repo with your values (ex. axcap/Esp-GitHub-OTA)
// This is a link to repo where your firmware updates will be pulled from
// #define RELEASE_URL "https://api.github.com/repos/AlejandroBalonga/Riego-y-sisterna-ESP8266/releases/latest"

// Use this version of the URL together with
// Line 22: GitHubOTA GitHubOTA(VERSION, RELEASE_URL, "firmware.bin", "filesystem.bin", "/.fs_update_pending", false);
// under debugging to spare yourself from getting timeout from GitHub API
#define RELEASE_URL "https://github.com/AlejandroBalonga/Riego-y-sisterna-ESP8266/releases/latest"

// Uncomment the line below when using Github Api to get the latest release (Line 9)
// GitHubOTA GitHubOTA(VERSION, RELEASE_URL);

GitHubOTA OsOta(VERSION, RELEASE_URL, "firmware.bin", true);
GitHubFsOTA FsOta(VERSION, RELEASE_URL, "filesystem.bin", true);

#define LED D4
#define TRIAC D5
#define IN_A D6
#define IN_B D7

// Set i2c address
PCF8574 pcf8574(0x20);

void listRoot();

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, true);
  pinMode(TRIAC, OUTPUT);
  digitalWrite(TRIAC, true);
  pinMode(IN_A, INPUT_PULLUP);
  pinMode(IN_B, INPUT_PULLUP);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Config Riego WIFI");

  Serial.printf("Current firmware version: %s\n", VERSION);
  Serial.printf("Current filesystem version: %s\n", VERSION);

  LittleFS.begin();
  listRoot();

  // Chech for updates
  Serial.printf("Chech for Firmware updates\r\n");
  OsOta.handle();
  Serial.printf("Chech for FileSistem updates\r\n");
  FsOta.handle();
  Serial.printf("fin del setup");
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
  digitalWrite(LED, !digitalRead(IN_A));
  digitalWrite(TRIAC, digitalRead(IN_B));
}

void listRoot()
{
  Serial.printf("Listing root directory\r\n");

  File root = LittleFS.open("/", "r");
  File file = root.openNextFile();

  while (file)
  {
    Serial.printf("  FILE: %s\r\n", file.name());
    file = root.openNextFile();
  }
}