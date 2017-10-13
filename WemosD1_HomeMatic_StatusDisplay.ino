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
#include "global_css.h"
#include "webConfigHTML.h"

#define                       SERIALDEBUG
//#define                       WM_DEBUG_OUTPUT
//#define                       UDPDEBUG

#define CONFIG_PIN            D1
#define PIR_PIN               D5
#define WEBSERVER_PORT        80
#define UDPPORT             6690
#define HTTPGETTIMEOUT      2000 // 2 Sekunden Timeout für Anfragen an die CCU
#define KEYPRESSLONGMILLIS  1500 // ms für langen Tastendruck
#define KEYBOUNCEMILLIS      200 // ms Mindestzeit zwischen 2 Tastendrücken
#define KEYTOLERANCE          10 // +/-Toleranz für 16er Widerstands-/Tastermatrix

#ifdef UDPDEBUG
const char * SYSLOGIP = "192.168.1.251";
#define SYSLOGUDPPORT   514
#endif

enum _SyslogSeverity {
  _slEmergency,
  _slAlert,
  _slCritical,
  _slError,
  _slWarning,
  _slNotice,
  _slInformational,
  _slDebug
};

// FastLED
#define LED_PIN D7
#define LED_TYPE WS2812
#define MAX_LEDS 255
struct led_t {
  CRGB leds[MAX_LEDS];
  CRGB ledsBackup[MAX_LEDS];
  int  Colors[MAX_LEDS];
  bool Blink[MAX_LEDS];
  int  Keys[16] {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
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
  "00FF00", "F39C12", "FF0000", "FFFFFF", "0000FF", "00FF00", "F39C12", "FF0000", "FFFFFF", "0000FF"
};

enum DisplayStates {
  Sleep,
  Wake
};

ESP8266WebServer WebServer(WEBSERVER_PORT);
ESP8266HTTPUpdateServer httpUpdater;

//UDP
struct udp_t {
  WiFiUDP UDP;
  char incomingPacket[255];
} UDPClient;

//WifiManager - don't touch
byte configPortalTimeout    = 180;
bool shouldSaveConfig       = false;
String configJsonFile   = "config.json";
char ip[IP_SIZE]             = "0.0.0.0";
char netmask[IP_SIZE]        = "0.0.0.0";
char gw[IP_SIZE]             = "0.0.0.0";
boolean startWifiManager    = false;

int DisplayTimeoutSeconds = 0;
unsigned long LastDisplayTimeOutMillis = 0;
unsigned long LastBlinkMillis = 0;
bool OTAStart = false;
bool DisplayState = Wake;
bool blinkState = false;
bool UDPReady = false;
volatile byte PIRInterruptDetected = 0;
bool ConfigKeyPress = false;
bool ConfigKeyPressLONG = false;
unsigned long ConfigKeyPressDownMillis = 0;
unsigned long LastMillisConfigKeyPress = 0;

void setup() {
  pinMode(A0, INPUT);
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  initPIR();

  digitalWrite(LED_BUILTIN, HIGH);
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  DEBUG("Programmstart...");

  if (digitalRead(CONFIG_PIN) == LOW) {
    DEBUG("Taster gedrückt - Config Mode wird gestartet!");
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
    DEBUG("WLAN erfolgreich verbunden!");
    printWifiStatus();
  } else ESP.restart();

  startWebServer();

  initUDPClient();

  initLEDs();

  showLedBootSequence();

  startOTAhandling();

  if (GlobalConfig.RestoreStateFromCCU)
    getValuesFromCCU();

  DEBUG(String(GlobalConfig.DeviceName) + " - Boot abgeschlossen, SSID = " + WiFi.SSID() + ", IP = " + String(IpAddress2String(WiFi.localIP())) + ", RSSI = " + WiFi.RSSI() + ", MAC = " + WiFi.macAddress(), "Setup", _slInformational);
}

void loop() {
  if (LastDisplayTimeOutMillis > millis())
    LastDisplayTimeOutMillis = millis();
  if (LastBlinkMillis > millis())
    LastBlinkMillis = millis();

  ArduinoOTA.handle();

  if (!OTAStart) {
    WebServer.handleClient();

    handleUDP();

    handlePIR();

    handleKEY();

    if (DisplayTimeoutSeconds > 0 && DisplayState == Wake && millis() - LastDisplayTimeOutMillis > DisplayTimeoutSeconds * 1000) {
      LastDisplayTimeOutMillis = millis();
      DEBUG("Display Timeout", "loop()", _slInformational);
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

    //Tasterbedienung CONFIG_PIN
    if (digitalRead(CONFIG_PIN) == LOW) {
      if (!ConfigKeyPress) {
        ConfigKeyPressDownMillis = millis();
        if (millis() - LastMillisConfigKeyPress > KEYBOUNCEMILLIS) {
          LastMillisConfigKeyPress = millis();
          ConfigKeyPress = true;
        }
      }

      if ((millis() - ConfigKeyPressDownMillis) > KEYPRESSLONGMILLIS && !ConfigKeyPressLONG) {
        //PRESS_LONG
        DEBUG("ConfigKeyPress LONG", "loop()", _slInformational);
        ResistorConfigKeys();
        ConfigKeyPressLONG = true;
      }
    } else {
      if (ConfigKeyPress) {
        if ((millis() - ConfigKeyPressDownMillis) < KEYPRESSLONGMILLIS) {
          int brightness = GlobalConfig.LedBrightness;
          brightness = brightness + 25;
          if (brightness > 255) brightness = 10;
          DEBUG("ConfigKeyPress SHORT, setting brightness to " + String(brightness), "loop()", _slInformational);
          setLedBrightness(brightness);
          delay(10); //Entprellen
        }
      }
      ConfigKeyPress = false;
      ConfigKeyPressLONG = false;
    }
    delay(10); //wg. UDP
  }
}
