void initLEDs() {
  Serial.print("\nEOrder = ");
  switch (GlobalConfig.SelectedEOrder) {
    case _RGB:
      Serial.println("RGB");
      FastLED.addLeds<LED_TYPE, LED_PIN, RGB>(leds, GlobalConfig.NumLeds);
      break;
    case _GRB:
      Serial.println("GRB");
      FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, GlobalConfig.NumLeds);
      break;
    default:
      Serial.println("GRB");
      FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, GlobalConfig.NumLeds);
      break;
  }
  FastLED.setBrightness(GlobalConfig.LedBrightness);
}

void showLedBootSequence() {
  int bootColors[] = {CRGB::Black, CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black};

  for (int i = 0; i < sizeof(bootColors) / sizeof(int); i++) {
    fill_solid(leds, GlobalConfig.NumLeds, bootColors[i]);
    FastLED.show();
    delay(500);
  }
}

void setLedBrightness(byte brightness) {
  setLedMode(Sleep);
  FastLED.setBrightness(brightness);
  setLedMode(Wake);
  GlobalConfig.LedBrightness = brightness;
  saveSystemConfig();
}

void setLed(int led, int hex) {
  Serial.println("setLed for LED " + String(led) + " value = " + String(hex, HEX));
  if (DisplayState == Wake) {
    if (led == -1) {
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        leds[i] = hex;
        ledColors[i] = hex;
      }
    } else {
      leds[led] = hex;
      ledColors[led] = hex;
    }
  } else {
    if (led == -1)  {
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        ledsBackup[i] = hex;
        ledColors[i] = hex;
      }
    } else {
      ledsBackup[led] = hex;
      ledColors[led] = hex;
    }
  }
  SomeLedHasChanged = true;
}

void setLedMode(byte Mode) {
  setLedMode(Mode, 0);
}
void setLedMode(byte Mode, int _DisplayTimeoutSeconds) {
  if (DisplayState != Mode) {
    DisplayState = Mode;
    if (Mode == Sleep) {
      Serial.println("setLedMode to SLEEP");
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        ledsBackup[i] = leds[i];
      }
      fill_solid(leds, GlobalConfig.NumLeds, CRGB::Black);
    } else {
      DisplayTimeoutSeconds = _DisplayTimeoutSeconds;
      Serial.println("setLedMode to WAKE with Display Timeout " + String(DisplayTimeoutSeconds) + " seconds");
      if (DisplayTimeoutSeconds > 0 )
        LastDisplayTimeOutMillis = millis();

      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        leds[i] = ledsBackup[i];
      }
    }
    FastLED.show();
  }
}



