unsigned long AnalogReadDebugMillis = 0;
unsigned long KeyPressDownMillis = 0;
unsigned long LastMillisKeyPress = 0;
bool KeyPress = false;
byte KeyDetected = 0;
bool PRESS_LONGsent = false;
String KeyChannelName = "";

void handleKEY() {
  int aVal = analogRead(A0);

  if (millis() - AnalogReadDebugMillis > 1000) {
    AnalogReadDebugMillis = millis();
    //DEBUG("analogRead Value = " + String(aVal), "handleKEY()", _slInformational);
  }

  for (int i = 0; i < 16; i++) {
    if (aVal < LEDConfig.Keys[i] + KEYTOLERANCE && aVal > LEDConfig.Keys[i] - KEYTOLERANCE) {
      KeyDetected = i + 1;
      break;
    } else {
      KeyDetected = 0;
    }
  }

  if (KeyDetected > 0) {
    if (!KeyPress) {
      KeyPressDownMillis = millis();
      if (millis() - LastMillisKeyPress > KEYBOUNCEMILLIS) {
        DEBUG("KEY Number " + String(KeyDetected) + " detected", "handleKEY()", _slInformational);
        LastMillisKeyPress = millis();
        KeyPress = true;
      }
    }

    if ((millis() - KeyPressDownMillis) > KEYPRESSLONGMILLIS && !PRESS_LONGsent) {
      //PRESS_LONG
      DEBUG("PRESS_LONG", "handleKEY()", _slInformational);
      KeyChannelName =  getStateCUxD(String(GlobalConfig.DeviceName) + "Taster:" + String(KeyDetected), "Address");
      DEBUG("KeyChannelName = " + KeyChannelName, "handleKEY()", _slInformational);
      if (KeyChannelName != "") {
        setStateCUxD("CUxD." + KeyChannelName + ".PRESS_LONG", "true");
      } else {
        DEBUG("CUxD Tasterkanal nicht gefunden!", "handleKEY()", _slError);
      }
      PRESS_LONGsent = true;
    }

  } else {
    if (KeyPress) {
      if ((millis() - KeyPressDownMillis) < KEYPRESSLONGMILLIS) {
        //PRESS_SHORT
        DEBUG("PRESS_SHORT", "handleKEY()", _slInformational);
        KeyChannelName =  getStateCUxD(String(GlobalConfig.DeviceName) + "Taster:" + String(KeyDetected), "Address");
        DEBUG("KeyChannelName = " + KeyChannelName, "handleKEY()", _slInformational);
        if (KeyChannelName != "") {
          setStateCUxD("CUxD." + KeyChannelName + ".PRESS_SHORT", "true");
        } else {
          DEBUG("CUxD Tasterkanal nicht gefunden!", "handleKEY()", _slError);
        }
      }
    }
    KeyPress = false;
    PRESS_LONGsent = false;
  }
}

