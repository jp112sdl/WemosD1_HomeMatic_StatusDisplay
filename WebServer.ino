const char HTTP_TITLE_LABEL[] PROGMEM = "<div class='l lt'><label>{v}</label><hr /></div>";
const char HTTP_CONFIG_BUTTON[] PROGMEM = "<div></div><hr /><div></div><div><input class='lnkbtn' type='button' value='Konfiguration' onclick=\"window.location.href='/config'\" /></div><div><input class='lnkbtn' type='button' value='Firmware Update' onclick=\"window.location.href='/update'\" /></div>";
const char HTTP_TABLE_START[] PROGMEM = "<table class=tc>";
const char HTTP_TABLE_END[] PROGMEM = "</table>";
const char HTTP_SAVE_BUTTON[] PROGMEM = "<div><button name='btnSave' value='1' type='submit'>Speichern</button></div>";
const char HTTP_HOME_BUTTON[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Zur&uuml;ck' onclick=\"window.location.href='/'\" /></div>";

void startWebServer() {
  WebServer.on("/set", webSetLed);
  WebServer.on("/getValuesFromCCU", []() {
    WebServer.send(200, "text/plain", "<getValuesFromCCU>ok</getValuesFromCCU>");
    getValuesFromCCU();
  });
  WebServer.on("/sleep", webSleep);
  WebServer.on("/wakeup", webWakeup);
  WebServer.on("/wifiStatus", webWifiStatus);
  WebServer.on("/config", webConfig);
  WebServer.onNotFound([]() {
    WebServer.send(404, "text/plain", "Hier gibt es nichts zu sehen!");
  });
  WebServer.on("/", webDefaultHtml);
  httpUpdater.setup(&WebServer);
  WebServer.begin();
  DEBUG("Starte Webserver an Port " + String(WEBSERVER_PORT) + "...", "startWebServer()", _slInformational);
}

void webDefaultHtml() {
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SD_STYLE);
  page += FPSTR(HTTP_ALL_STYLE);
  page += FPSTR(HTTP_HM_STYLE);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<div class='fbg'>");
  page += FPSTR(HTTP_TITLE_LABEL);

  page += FPSTR(HTTP_TABLE_START);

  String _table = "";

  byte rows = GlobalConfig.NumLeds / 2;
  if (GlobalConfig.NumLeds % 2 != 0) {
    rows = (GlobalConfig.NumLeds + 1) / 2;
  }
  for (int row = 1; row < rows + 1; row ++) {

    _table += "<tr><td bgcolor={c" + String(row) + "}>" + ((LEDConfig.Blink[row - 1]) ? "<center>B</center>" : "") + "</td><td>" + String(row) + "</td>";
    if ((rows + row) > GlobalConfig.NumLeds) {
      _table += "</tr>";
    } else {
      _table += "<td align='right'>" + String(rows + row) + "</td><td bgcolor={c" + String(rows + row) + "}>" + ((LEDConfig.Blink[(rows + row) - 1]) ? "<center>B</center>" : "") + "</td></tr>";
    }
  }

  page += _table;

  page += FPSTR(HTTP_TABLE_END);

  for (int i = 0; i < GlobalConfig.NumLeds; i++) {
    String hexCol =  String(LEDConfig.Colors[i], HEX);
    for (int i = 0; 6 - hexCol.length(); i++)
      hexCol = "0" + hexCol;
    //DEBUG("WEB-Farbe LED " + String(i) + " = " + hexCol, "webDefaultHtml()", _slInformational);
    page.replace("{c" + String(i + 1) + "}", hexCol);
  }
  page += FPSTR(HTTP_CONFIG_BUTTON);

  page += F("</div></div></body></html>");
  page.replace("{v}", GlobalConfig.DeviceName);
  WebServer.sendHeader("Content-Length", String(page.length()));
  WebServer.send(200, "text/html\r\nRefresh:10", page);
}

void webSetLed() {
  int led = 0;
  int hex = 0;
  int r   = 0;
  int g   = 0;
  int b   = 0;
  bool useHex = false;
  bool blink = false;
  bool webSetLEDBrightness = false;

  char hexArray[HEXARRAY_SIZE];

  if (WebServer.args() > 0) {
    for (int i = 0; i < WebServer.args(); i++) {
      if (WebServer.argName(i) == "led") {
        led = WebServer.arg(i).toInt();
        led = led - 1;
      }
      if (WebServer.argName(i) == "blink") {
        blink = WebServer.arg(i).toInt();
      }
      if (WebServer.argName(i) == "r")
        r = WebServer.arg("r").toInt();
      if (WebServer.argName(i) == "g")
        g = WebServer.arg("g").toInt();
      if (WebServer.argName(i) == "b")
        b = WebServer.arg("b").toInt();
      if (WebServer.argName(i) == "brightness") {
        byte brightness = WebServer.arg("brightness").toInt();
        setLedBrightness(brightness);
        webSetLEDBrightness = true;
      }
      if (WebServer.argName(i) == "hex") {
        String hexValue = WebServer.arg("hex");
        hexValue.replace("#", "");
        hexValue.toUpperCase();
        hexValue.toCharArray(hexArray, HEXARRAY_SIZE);
        hex = x2i(hexArray);
        useHex = true;
      }
      if (WebServer.argName(i) == "dim") {
        int dimVal = WebServer.arg(i).toInt();
        DEBUG("Got Dimmer for LED " + String(led) + " value = " + String(dimVal), "webSetLed()", _slInformational);
        blink = (dimVal >= GlobalConfig.DimBlink);
        hex = dim2val(dimVal);
        useHex = true;
      }
    }

    if (!webSetLEDBrightness) {
      if (!useHex)
        hex = ((int)r << 16L) | ((int)g << 8L) | (int)b;
      setLed(led, hex);
      LEDConfig.Blink[led] = blink;
    }
    WebServer.send(200, "text/plain", "<state>ok</state>");
  }
}

void webSleep() {
  setLedMode(Sleep);
  WebServer.send(200, "text/plain", "<state>ok</state>");
}

void webWakeup() {
  int _DisplayTimeoutSeconds = 0;
  if (WebServer.args() > 0) {
    for (int i = 0; i < WebServer.args(); i++) {
      if (WebServer.argName(i) == "t") {
        _DisplayTimeoutSeconds = WebServer.arg(i).toInt();
        DEBUG("wake up for " + String(_DisplayTimeoutSeconds) + " seconds", "webWakeup()", _slInformational);
      }
    }
  }

  setLedMode(Wake, _DisplayTimeoutSeconds);

  WebServer.send(200, "text/plain", "<state>ok</state>");
}

void webWifiStatus() {
  IPAddress ip = WiFi.localIP();
  WebServer.send(200, "text/plain", "<state>connected</state><ssid>" + WiFi.SSID() + "</ssid><ip>" + String(ip) + "</ip><rssi>" + WiFi.RSSI() + "</rssi><mac>" + WiFi.macAddress() + "</mac>");
}

void webConfig() {
  bool sc = false;

  if (WebServer.args() > 0) {
    for (int i = 0; i < WebServer.args(); i++) {
      DEBUG("Arg(" + String(i) + ") = " + WebServer.argName(i) + " [" + WebServer.arg(i) + "]", "webConfig()", _slInformational);
      if (WebServer.argName(i) == "ccuip")
        strcpy(GlobalConfig.CcuIp, WebServer.arg(i).c_str());

      if (WebServer.argName(i) == "devicename")
        strcpy(GlobalConfig.DeviceName, WebServer.arg(i).c_str());

      if (WebServer.argName(i) == "restorestate")
        GlobalConfig.RestoreStateFromCCU = WebServer.arg(i).toInt();

      if (WebServer.argName(i) == "numleds")
        GlobalConfig.NumLeds = WebServer.arg(i).toInt();

      if (WebServer.argName(i) == "rgborder")
        GlobalConfig.SelectedEOrder = WebServer.arg(i).toInt();

      if (WebServer.argName(i) == "dimblink")
        GlobalConfig.DimBlink = WebServer.arg(i).toInt();

      for (int i = 0 ; i < 10; i++) {
        if (WebServer.argName(i) == "color"+String(i+1))
          Dimmer2ColorDefinition[i] = WebServer.arg(i);
      }

      if (WebServer.argName(i) == "btnSave")
        sc = (WebServer.arg(i).toInt() == 1);
    }
  }

  if (sc) {
    saveSystemConfig();
    getValuesFromCCU();
  }

  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_ALL_STYLE);
  page += FPSTR(HTTP_HM_STYLE);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_HEAD_END);
  page += F("<div class='fbg'>");
  page += F("<form method='post' action='config'>");
  page += FPSTR(HTTP_TITLE_LABEL);
  page += FPSTR(HTTP_CONF);
  page += FPSTR(HTTP_SAVE_BUTTON);
  page += FPSTR(HTTP_HOME_BUTTON);
  page += F("</form></div>");
  page += FPSTR(HTTP_END);

  page.replace("{ccuip}", GlobalConfig.CcuIp);
  page.replace("{ckrestore}", GlobalConfig.RestoreStateFromCCU ? "checked" : "");
  page.replace("{numleds}", String(GlobalConfig.NumLeds));
  page.replace("{dimblink}", String(GlobalConfig.DimBlink));
  page.replace("{v}", GlobalConfig.DeviceName);
  switch (GlobalConfig.SelectedEOrder) {
    case _RGB:
      page.replace("{srgb}", "selected"); page.replace("{sgrb}", "");
      break;
    case _GRB:
      page.replace("{srgb}", ""); page.replace("{sgrb}", "selected");
    default: // GRB
      page.replace("{srgb}", ""); page.replace("{sgrb}", "selected");
      break;
  }

  for (int i = 0; i < COLOR_COUNT; i++) {
    page.replace("{color" + String(i + 1) + "}", Dimmer2ColorDefinition[i]);
  }
  WebServer.send(200, "text/html", page);
}

