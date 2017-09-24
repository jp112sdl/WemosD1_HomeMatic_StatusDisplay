const char HTTP_SD_STYLE[] PROGMEM = "<style>table.tc td {  width: 60px; padding:10px;}</style>";
const char HTTP_CONFIG_BUTTON[] PROGMEM = "<div></div><hr /><div></div><div><input class='lnkbtn' type='button' value='Konfiguration' onclick=\"window.location.href='/config'\" /></div>";
const char HTTP_TITLE_LABEL[] PROGMEM = "<div class='l lt'><label>{v}</label><hr /></div>";
const char HTTP_ALL_STYLE[] PROGMEM = "<style>div {white-space: nowrap;} input[type=text] {width:95%;} .green {color:green;} .red {color:red;} .tdr {float:right;} .tdl { width: 1px;} input.lnkbtn,input.fwbtn {-webkit-appearance: button;-moz-appearance: button;appearance: button;} body {background-color: #303030;} input.lnkbtn,button,input.fwbtn{cursor: pointer;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;padding:5px;} input,button,input.lnkbtn,input.fwbtn {border: 0;border-radius: 0.3rem;} .c{text-align: center;} .k{font-style:italic;} .fbg {background-color: #eee;} div,input{padding:5px;font-size:1em;} .i{text-align: right; width: 45% !important;} body{text-align: center;font-family:verdana;} .l{no-repeat left center;background-size: 1em;} .q{float: right;width: 64px;text-align: right;} .ls {font-weight: bold;text-align: center;font-size: 300%;} .lt{font-size: 150%;text-align: center;} table{width:100%;} td{max-width:50%;font-weight: bold;} input.fwbtn {display: none; background-color: #ff0000;} ";
const char HTTP_HM_STYLE[]  PROGMEM = "input.lnkbtn,button{background-color:#1fa3ec;}</style>";
const char HTTP_TABLE[] PROGMEM = "<table class=tc><tr><td>Status 1</td><td bgcolor={c1}></td><td>Status 9</td><td bgcolor={c9}></td></tr><tr><td>Status 2</td><td bgcolor={c2}></td><td>Status 10</td><td bgcolor={c10}></td></tr><tr><td>Status 3</td><td bgcolor={c3}></td><td>Status 11</td><td bgcolor={c11}></td></tr><tr><td>Status 4</td><td bgcolor={c4}></td><td>Status 12</td><td bgcolor={c12}></td></tr><tr><td>Status 5</td><td bgcolor={c5}></td><td>Status 13</td><td bgcolor={c13}></td></tr><tr><td>Status 6</td><td bgcolor={c6}></td><td>Status 14</td><td bgcolor={c14}></td></tr><tr><td>Status 7</td><td bgcolor={c7}></td><td>Status 15</td><td bgcolor={c15}></td></tr><tr><td>Status 8</td><td bgcolor={c8}></td><td>Status 16</td><td bgcolor={c16}></td></tr></table>";
const char HTTP_CONF[] PROGMEM = "<div>HomeMatic Konfiguration:</div><div id='div_ccu'><input type='text' id='ccuip' name='ccuip' length=1 placeholder='IP der CCU2' value='{ccuip}' pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'></div><div id='div_devicename'><input type='text' id='devicename' name='devicename' length=1 placeholder='Ger&auml;tename' value='{v}' pattern='\\w{1,}'></div><div id='div_restorestate'><label for='restorestate'>Boot: Lade Werte von CCU</label><span class='ckb'><input id='restorestate' type='checkbox' name='restorestate' {ckrestore} value=1></span></div><div>LED Konfiguration:</div><div id='div_numleds'><input type='text' id='numleds' name='numleds' length=1 placeholder='Anzahl LEDs' value='{numleds}' required pattern='[0-9]{1,2}'></div><div id='div_rgborder'><label for='rgborder'>RGB Reihenfolge</label><span class='cob ckb'><select id='rgborder' name='rgborder'><option {srgb} value='10'>RGB</option><option {sgrb} value='66'>GRB</option></select></span></div><div>Dimmer-Konfiguration:</div><div id='div_color1'><input type='input' id='color1' name='color1' length=1 placeholder='Farbe f&uuml;r Dimmer 10..19%' value='{color1}' onchange='setColor()' > <input type='color' id='cp_color1' name='cp_color1' onchange='setCPColor()'></div><div id='div_color2'><input type='input' id='color2' name='color2' length=1 placeholder='Farbe f&uuml;r Dimmer 20..29%' value='{color2}' onchange='setColor()' > <input type='color' id='cp_color2' name='cp_color2' onchange='setCPColor()'></div><div id='div_color3'><input type='input' id='color3' name='color3' length=1 placeholder='Farbe f&uuml;r Dimmer 30..39%' value='{color3}' onchange='setColor()' > <input type='color' id='cp_color3' name='cp_color3' onchange='setCPColor()'></div><div id='div_color4'><input type='input' id='color4' name='color4' length=1 placeholder='Farbe f&uuml;r Dimmer 40..49%' value='{color4}' onchange='setColor()' > <input type='color' id='cp_color4' name='cp_color4' onchange='setCPColor()'></div><div id='div_color5'><input type='input' id='color5' name='color5' length=1 placeholder='Farbe f&uuml;r Dimmer 50..59%' value='{color5}' onchange='setColor()' > <input type='color' id='cp_color5' name='cp_color5' onchange='setCPColor()'></div><div id='div_color6'><input type='input' id='color6' name='color6' length=1 placeholder='Farbe f&uuml;r Dimmer 60..69%' value='{color6}' onchange='setColor()' > <input type='color' id='cp_color6' name='cp_color6' onchange='setCPColor()'></div><div id='div_color7'><input type='input' id='color7' name='color7' length=1 placeholder='Farbe f&uuml;r Dimmer 70..79%' value='{color7}' onchange='setColor()' > <input type='color' id='cp_color7' name='cp_color7' onchange='setCPColor()'></div><div id='div_color8'><input type='input' id='color8' name='color8' length=1 placeholder='Farbe f&uuml;r Dimmer 80..89%' value='{color8}' onchange='setColor()' > <input type='color' id='cp_color8' name='cp_color8' onchange='setCPColor()'></div><div id='div_color9'><input type='input' id='color9' name='color9' length=1 placeholder='Farbe f&uuml;r Dimmer 90..99%' value='{color9}' onchange='setColor()' > <input type='color' id='cp_color9' name='cp_color9' onchange='setCPColor()'></div><div id='div_color10'><input type='input' id='color10' name='color10' length=1 placeholder='Farbe f&uuml;r Dimmer 100%' value='{color10}' onchange='setColor()' > <input type='color' id='cp_color10' name='cp_color10' onchange='setCPColor()'></div><div>Blinken f&uuml;r Dimmer-Werte ab...<br/>(11 = kein blinken)</div><div id='div_dimblink'><input type='text' id='dimblink' name='dimblink' length=1 placeholder='Blinken f&uuml;r Dimmer-Werte ab...' value='{dimblink}' required pattern='[0-9]{1,2}'></div>";
const char HTTP_SAVE_BUTTON[] PROGMEM = "<div><button name='btnSave' value='1' type='submit'>Speichern</button></div>";
const char HTTP_HOME_BUTTON[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Zur&uuml;ck' onclick=\"window.location.href='/'\" /></div>";

void startWebServer() {
  webServer.on("/set", webSetLed);
  webServer.on("/getValuesFromCCU", getValuesFromCCU);
  webServer.on("/sleep", webSleep);
  webServer.on("/wakeup", webWakeup);
  webServer.on("/wifiStatus", webWifiStatus);
  webServer.on("/config", webConfig);
  webServer.onNotFound([]() {
    webServer.send(404, "text/plain", "Hier gibt es nichts zu sehen!");
  });
  webServer.on("/", webDefaultHtml);
  webServer.begin();
  Serial.println("Starte Webserver an Port " + String(WEBSERVER_PORT) + "...");
}

void webDefaultHtml() {
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SD_STYLE);
  page += F("<body>");
  page += FPSTR(HTTP_TABLE);
  for (int i = 0; i < GlobalConfig.NumLeds; i++) {
    String hexCol =  String(LEDConfig.Colors[i], HEX);
    for (int i = 0; 6 - hexCol.length(); i++)
      hexCol = "0" + hexCol;
    page.replace("{c" + String(i + 1) + "}", hexCol);
  }
  page += FPSTR(HTTP_CONFIG_BUTTON);

  page += F("</body></html>");
  page.replace("{v}", GlobalConfig.DeviceName);
  webServer.sendHeader("Content-Length", String(page.length()));
  webServer.send(200, "text/html", page);
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

  if (webServer.args() > 0) {
    for (int i = 0; i < webServer.args(); i++) {
      if (webServer.argName(i) == "led") {
        led = webServer.arg(i).toInt();
        led = led - 1;
      }
      if (webServer.argName(i) == "blink") {
        blink = webServer.arg(i).toInt();
      }
      if (webServer.argName(i) == "r")
        r = webServer.arg("r").toInt();
      if (webServer.argName(i) == "g")
        g = webServer.arg("g").toInt();
      if (webServer.argName(i) == "b")
        b = webServer.arg("b").toInt();
      if (webServer.argName(i) == "brightness") {
        byte brightness = webServer.arg("brightness").toInt();
        setLedBrightness(brightness);
        webSetLEDBrightness = true;
      }
      if (webServer.argName(i) == "hex") {
        String hexValue = webServer.arg("hex");
        hexValue.replace("#", "");
        hexValue.toUpperCase();
        hexValue.toCharArray(hexArray, HEXARRAY_SIZE);
        hex = x2i(hexArray);
        useHex = true;
      }
      if (webServer.argName(i) == "dim") {
        int dimVal = webServer.arg(i).toInt();
        Serial.println("Got Dimmer Value " + String(dimVal) + " for LED " + String(led));
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
    webServer.send(200, "text/plain", "<state>ok</state>");
  }
}

void webSleep() {
  setLedMode(Sleep);
  webServer.send(200, "text/plain", "<state>ok</state>");
}

void webWakeup() {
  int _DisplayTimeoutSeconds = 0;
  if (webServer.args() > 0) {
    for (int i = 0; i < webServer.args(); i++) {
      if (webServer.argName(i) == "t") {
        _DisplayTimeoutSeconds = webServer.arg(i).toInt();
        Serial.println("wake up for " + String(_DisplayTimeoutSeconds) + " seconds");
      }
    }
  }

  setLedMode(Wake, _DisplayTimeoutSeconds);

  webServer.send(200, "text/plain", "<state>ok</state>");
}

void webWifiStatus() {
  IPAddress ip = WiFi.localIP();
  webServer.send(200, "text/plain", "<state>connected</state><ssid>" + WiFi.SSID() + "</ssid><ip>" + String(ip) + "</ip><rssi>" + WiFi.RSSI() + "</rssi><mac>" + WiFi.macAddress() + "</mac>");
}

void webConfig() {
  bool sc = false;

  if (webServer.args() > 0) {
    for (int i = 0; i < webServer.args(); i++) {
      Serial.println("Arg(" + String(i) + ") = " + webServer.argName(i) + " [" + webServer.arg(i) + "]");
      if (webServer.argName(i) == "ccuip")
        strcpy(GlobalConfig.CcuIp, webServer.arg(i).c_str());

      if (webServer.argName(i) == "devicename")
        strcpy(GlobalConfig.DeviceName, webServer.arg(i).c_str());

      if (webServer.argName(i) == "restorestate")
        GlobalConfig.RestoreStateFromCCU = webServer.arg(i).toInt();

      if (webServer.argName(i) == "numleds")
        GlobalConfig.NumLeds = webServer.arg(i).toInt();

      if (webServer.argName(i) == "rgborder")
        GlobalConfig.SelectedEOrder = webServer.arg(i).toInt();

      if (webServer.argName(i) == "dimblink")
        GlobalConfig.DimBlink = webServer.arg(i).toInt();

      if (webServer.argName(i) == "color1")
        Dimmer2ColorDefinition[0] = webServer.arg(i);

      if (webServer.argName(i) == "color2")
        Dimmer2ColorDefinition[1] = webServer.arg(i);

      if (webServer.argName(i) == "color3")
        Dimmer2ColorDefinition[2]  = webServer.arg(i);

      if (webServer.argName(i) == "color4")
        Dimmer2ColorDefinition[3]  = webServer.arg(i);

      if (webServer.argName(i) == "color5")
        Dimmer2ColorDefinition[4]  = webServer.arg(i);

      if (webServer.argName(i) == "color6")
        Dimmer2ColorDefinition[5]  = webServer.arg(i);

      if (webServer.argName(i) == "color7")
        Dimmer2ColorDefinition[6]  = webServer.arg(i);

      if (webServer.argName(i) == "color8")
        Dimmer2ColorDefinition[7]  = webServer.arg(i);

      if (webServer.argName(i) == "color9")
        Dimmer2ColorDefinition[8]  = webServer.arg(i);

      if (webServer.argName(i) == "color10")
        Dimmer2ColorDefinition[9]  = webServer.arg(i);

      if (webServer.argName(i) == "btnSave")
        sc = (webServer.arg(i).toInt() == 1);
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
  webServer.send(200, "text/html", page);
}

