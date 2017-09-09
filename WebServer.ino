const char HTTP_SD_STYLE[] PROGMEM = "<style>table.tc td {  width: 60px; padding:10px;}</style>";
const char HTTP_TABLE[] PROGMEM = "<table class=tc><tr><td>Status 1</td><td bgcolor={c1}></td><td>Status 9</td><td bgcolor={c9}></td></tr><tr><td>Status 2</td><td bgcolor={c2}></td><td>Status 10</td><td bgcolor={c10}></td></tr><tr><td>Status 3</td><td bgcolor={c3}></td><td>Status 11</td><td bgcolor={c11}></td></tr><tr><td>Status 4</td><td bgcolor={c4}></td><td>Status 12</td><td bgcolor={c12}></td></tr><tr><td>Status 5</td><td bgcolor={c5}></td><td>Status 13</td><td bgcolor={c13}></td></tr><tr><td>Status 6</td><td bgcolor={c6}></td><td>Status 14</td><td bgcolor={c14}></td></tr><tr><td>Status 7</td><td bgcolor={c7}></td><td>Status 15</td><td bgcolor={c15}></td></tr><tr><td>Status 8</td><td bgcolor={c8}></td><td>Status 16</td><td bgcolor={c16}></td></tr></table>";

void startWebServer() {
  webServer.on("/set", webSetLed);
  webServer.on("/sleep", webSleep);
  webServer.on("/wakeup", webWakeup);
  webServer.on("/wifiStatus", webWifiStatus);
  webServer.onNotFound([]() {
    webServer.send(404, "text/play", "Hier gibt es nichts zu sehen!");
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
    String hexCol =  String(ledColors[i],HEX);
    for (int i = 0; 6-hexCol.length(); i++)
        hexCol = "0"+hexCol;
    page.replace("{c" + String(i + 1) + "}",hexCol);
  }
  page += F("</body></html>");
  page.replace("{v}", GlobalConfig.deviceName);
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
  bool webSetLEDBrightness = false;

  char hexArray[HEXARRAY_SIZE];

  if (webServer.args() > 0) {
    for (int i = 0; i < webServer.args(); i++) {
      if (webServer.argName(i) == "led") {
        led = webServer.arg(i).toInt();
        led = led - 1;
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
        Serial.println("Got Dimmer Value " + String(dimVal));
        hex = dim2val(dimVal);
        useHex = true;
      }
    }

    if (!webSetLEDBrightness) {
      if (!useHex)
        hex = ((int)r << 16L) | ((int)g << 8L) | (int)b;
      setLed(led, hex);
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
