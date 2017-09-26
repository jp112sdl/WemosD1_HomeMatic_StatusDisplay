#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "WM_Custom.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>

#define CONFIG_PIN     D1 //Taster gegen GND, um den Konfigurationsmodus zu aktivieren
#define PIR_PIN        D5
#define WEBSERVER_PORT 80
#define UDPPORT        6690

// FastLED
#define LED_PIN D7
#define LED_TYPE WS2812
#define MAX_LEDS 255
struct led_t {
  CRGB leds[MAX_LEDS];
  CRGB ledsBackup[MAX_LEDS];
  int  Colors[MAX_LEDS];
  bool Blink[MAX_LEDS];
} LEDConfig;

enum _EOrder {
  _RGB = 0012,
  _RBG = 0021,
  _GRB = 0102,
  _GBR = 0120,
  _BRG = 0201,
  _BGR = 0210
};
#define IP_SIZE        16
#define VARIABLE_SIZE  100

struct globalconfig_t {
  byte LedBrightness = 255;
  int SelectedEOrder = _RGB;
  int NumLeds =        16;
  char CcuIp[IP_SIZE];
  char DeviceName[VARIABLE_SIZE];
  bool RestoreStateFromCCU = false;
  byte DimBlink = 11;
} GlobalConfig;

#define HEXARRAY_SIZE  24
#define COLOR_COUNT    10

String Dimmer2ColorDefinition[COLOR_COUNT] {
  "FF0000", "FFFF00", "00FF00", "8E44AD", "0000FF", "F39C12", "A3E4D7", "F5CBA7", "FFFFFF", "FFFFFF"
};

enum DisplayStates {
  Sleep,
  Wake
};

ESP8266WebServer webServer(WEBSERVER_PORT);
ESP8266HTTPUpdateServer httpUpdater;

//UDP
struct udp_t {
  WiFiUDP UDP;
  char incomingPacket[255];
} UDPClient;

//WifiManager - don't touch
byte configPortalTimeout    = 180;
bool shouldSaveConfig       = false;
String bootConfigJsonFile   = "config.json";
#define WM_DEBUG_OUTPUT  true
char ip[IP_SIZE]             = "0.0.0.0";
char netmask[IP_SIZE]        = "0.0.0.0";
char gw[IP_SIZE]             = "0.0.0.0";
boolean startWifiManager    = false;

int DisplayTimeoutSeconds = 0;
unsigned long LastDisplayTimeOutMillis = 0;
unsigned long LastBlinkMillis = 0;
bool OTAStart = false;
bool DisplayState = Wake;
bool brightnesskeylast = false;
bool blinkState = false;
volatile byte PIRInterruptDetected = 0;

void setup() {
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  initPIR();

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("Programmstart...");

  if (digitalRead(CONFIG_PIN) == LOW) {
    Serial.println("Taster gedrückt - Config Mode wird gestartet!");
    startWifiManager = true;
    bool state = LOW;
    for (int i = 0; i < 7; i++) {
      state = !state;
      digitalWrite(LED_BUILTIN, state);
      delay(100);
    }
  }
  loadSystemConfig();

  if (doWifiConnect()) {
    Serial.println("WLAN erfolgreich verbunden!");
    printWifiStatus();
  } else ESP.restart();

  startWebServer();

  initUDPClient();

  initLEDs();

  showLedBootSequence();

  startOTAhandling();

  if (GlobalConfig.RestoreStateFromCCU)
    getValuesFromCCU();

  Serial.println("Boot abgeschlossen");
}

void loop() {
  if (LastDisplayTimeOutMillis > millis())
    LastDisplayTimeOutMillis = millis();
  if (LastBlinkMillis > millis())
    LastBlinkMillis = millis();

  ArduinoOTA.handle();

  if (!OTAStart) {
    webServer.handleClient();

    handleUDP();

    handlePIR();

    if (DisplayTimeoutSeconds > 0 && DisplayState == Wake && millis() - LastDisplayTimeOutMillis > DisplayTimeoutSeconds * 1000) {
      LastDisplayTimeOutMillis = millis();
      Serial.println("Display Timeout");
      setLedMode(Sleep);
    }

    if (millis() - LastBlinkMillis > 1000 && DisplayState == Wake) {
      LastBlinkMillis = millis();
      blinkState = !blinkState;
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.ledsBackup[i] = LEDConfig.leds[i];
        if (blinkState && LEDConfig.Blink[i]) {
          LEDConfig.leds[i] = CRGB::Black;
        }
      }
      FastLED.show();
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.leds[i] = LEDConfig.ledsBackup[i];
      }
    }

    if (digitalRead(CONFIG_PIN) == LOW ) {
      if (!brightnesskeylast) {
        brightnesskeylast = true;
        int brightness = GlobalConfig.LedBrightness;
        brightness = brightness + 25;
        if (brightness > 255) brightness = 10;
        setLedBrightness(brightness);
        Serial.println("BRIGHTNESS_KEY pressed! Set brightness to " + String(brightness));
        delay(10); //Entprellen
      }
    } else {
      brightnesskeylast = false;
    }
    delay(10); //wg. UDP
  }
}
