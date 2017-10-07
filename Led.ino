void initLEDs() {
  Serial.print("\nEOrder = ");
  switch (GlobalConfig.SelectedEOrder) {
    case _RGB:
      DEBUG("RGB","initLEDs()",_slInformational);
      FastLED.addLeds<LED_TYPE, LED_PIN, RGB>(LEDConfig.leds, GlobalConfig.NumLeds);
      break;
    case _GRB:
      DEBUG("GRB","initLEDs()",_slInformational);
      FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(LEDConfig.leds, GlobalConfig.NumLeds);
      break;
    default:
      DEBUG("GRB","initLEDs()",_slInformational);
      FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(LEDConfig.leds, GlobalConfig.NumLeds);
      break;
  }
  FastLED.setBrightness(GlobalConfig.LedBrightness);
}

void showLedBootSequence() {
  int bootColors[] = {CRGB::Black, CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black};

  for (int i = 0; i < sizeof(bootColors) / sizeof(int); i++) {
    fill_solid(LEDConfig.leds, GlobalConfig.NumLeds, bootColors[i]);
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
  DEBUG("setLed for LED " + String(led) + " value = " + String(hex, HEX),"setLed()",_slInformational);
  if (DisplayState == Wake) {
    if (led == -1) {
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.leds[i] = hex;
        LEDConfig.Colors[i] = hex;
      }
    } else {
      LEDConfig.leds[led] = hex;
      LEDConfig.Colors[led] = hex;
    }
  } else {
    if (led == -1)  {
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.ledsBackup[i] = hex;
        LEDConfig.Colors[i] = hex;
      }
    } else {
      LEDConfig.ledsBackup[led] = hex;
      LEDConfig.Colors[led] = hex;
    }
  }
}

void setLedMode(byte Mode) {
  setLedMode(Mode, 0);
}
void setLedMode(byte Mode, int _DisplayTimeoutSeconds) {
  if (DisplayState != Mode) {
    DisplayState = Mode;
    if (Mode == Sleep) {
      DEBUG("setLedMode to SLEEP","setLedMode()",_slInformational);
      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.ledsBackup[i] = LEDConfig.leds[i];
      }
      fill_solid(LEDConfig.leds, GlobalConfig.NumLeds, CRGB::Black);
    } else {
      DisplayTimeoutSeconds = _DisplayTimeoutSeconds;
      DEBUG("setLedMode to WAKE with Display Timeout " + String(DisplayTimeoutSeconds) + " seconds","setLedMode()",_slInformational);
      if (DisplayTimeoutSeconds > 0 )
        LastDisplayTimeOutMillis = millis();

      for (int i = 0; i < GlobalConfig.NumLeds; i++) {
        LEDConfig.leds[i] = LEDConfig.ledsBackup[i];
      }
    }
    FastLED.show();
  }
}



