bool doWifiConnect() {
  String _ssid = WiFi.SSID();
  String _psk = WiFi.psk();

  const char* ipStr = ip; byte ipBytes[4]; parseBytes(ipStr, '.', ipBytes, 4, 10);
  const char* netmaskStr = netmask; byte netmaskBytes[4]; parseBytes(netmaskStr, '.', netmaskBytes, 4, 10);
  const char* gwStr = gw; byte gwBytes[4]; parseBytes(gwStr, '.', gwBytes, 4, 10);
  if (!startWifiManager && _ssid != "" && _psk != "" ) {
    Serial.println(F("Connecting WLAN the classic way..."));
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.hostname(GlobalConfig.deviceName);
    WiFi.begin(_ssid.c_str(), _psk.c_str());
    int waitCounter = 0;
    if (String(ip) != "0.0.0.0")
      WiFi.config(IPAddress(ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]), IPAddress(gwBytes[0], gwBytes[1], gwBytes[2], gwBytes[3]), IPAddress(netmaskBytes[0], netmaskBytes[1], netmaskBytes[2], netmaskBytes[3]));

    while (WiFi.status() != WL_CONNECTED) {
      waitCounter++;
      Serial.print(".");
      if (waitCounter == 20) {
        return false;
      }
      delay(500);
    }
    Serial.println("Wifi Connected");
    return true;
  } else {
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(WM_DEBUG_OUTPUT);
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    WiFiManagerParameter custom_text_hm("<br>HomeMatic Konfiguration:");
    WiFiManagerParameter custom_ccuip("ccu", "IP der CCU2", GlobalConfig.ccuIp, IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_devicename("devicename", "Ger&auml;tename", GlobalConfig.deviceName, VARIABLE_SIZE, INPUT_TEXT, "pattern='\\w{1,}'");
    char*chrRestoreOldState = "0";
    if (GlobalConfig.restoreStateFromCCU) chrRestoreOldState =  "1" ;
    WiFiManagerParameter custom_cbrestorestate("restorestate", "Boot: Lade Werte von CCU", chrRestoreOldState, 8, INPUT_CHECKBOX);

    WiFiManagerParameter custom_text_led("<br/><br>LED Konfiguration:");
    WiFiManagerParameter custom_numleds("numleds", "Anzahl LEDs", String(GlobalConfig.NumLeds).c_str(), VARIABLE_SIZE, INPUT_TEXT, "required pattern='[0-9]{1,2}'");

    String eorder = "<option {srgb} value='" + String(_RGB) + "'>RGB</option><option {sgrb} value='" + String(_GRB) + "'>GRB</option>";

    switch (GlobalConfig.SelectedEOrder) {
      case _RGB:
        eorder.replace("{srgb}", "selected"); eorder.replace("{sgrb}", "");
        break;
      case _GRB:
        eorder.replace("{srgb}", ""); eorder.replace("{sgrb}", "selected");
      default: // GRB
        eorder.replace("{srgb}", ""); eorder.replace("{sgrb}", "selected");
        break;
    }
    WiFiManagerParameter custom_rgborder("rgborder", "RGB Reihenfolge", "", 8, INPUT_COMBOBOX, eorder.c_str());

    WiFiManagerParameter custom_color1("color1", "Farbe f&uuml;r Dimmer 10..19%", Dimmer2ColorDefinition[0].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color2("color2", "Farbe f&uuml;r Dimmer 20..29%", Dimmer2ColorDefinition[1].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color3("color3", "Farbe f&uuml;r Dimmer 30..39%", Dimmer2ColorDefinition[2].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color4("color4", "Farbe f&uuml;r Dimmer 40..49%", Dimmer2ColorDefinition[3].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color5("color5", "Farbe f&uuml;r Dimmer 50..59%", Dimmer2ColorDefinition[4].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color6("color6", "Farbe f&uuml;r Dimmer 60..69%", Dimmer2ColorDefinition[5].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color7("color7", "Farbe f&uuml;r Dimmer 70..79%", Dimmer2ColorDefinition[6].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color8("color8", "Farbe f&uuml;r Dimmer 80..89%", Dimmer2ColorDefinition[7].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color9("color9", "Farbe f&uuml;r Dimmer 90..99%", Dimmer2ColorDefinition[8].c_str(), VARIABLE_SIZE, INPUT_COLOR);
    WiFiManagerParameter custom_color10("color10", "Farbe f&uuml;r Dimmer 100%", Dimmer2ColorDefinition[9].c_str(), VARIABLE_SIZE, INPUT_COLOR);

    WiFiManagerParameter custom_text_dhcp("<br/><br>Statische IP (wenn leer, dann DHCP):");
    WiFiManagerParameter custom_ip("custom_ip", "IP-Adresse", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_netmask("custom_netmask", "Netzmaske", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_gw("custom_gw", "Gateway", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");

    wifiManager.addParameter(&custom_text_hm);
    wifiManager.addParameter(&custom_ccuip);
    wifiManager.addParameter(&custom_devicename);
    wifiManager.addParameter(&custom_cbrestorestate);

    wifiManager.addParameter(&custom_text_led);
    wifiManager.addParameter(&custom_numleds);
    wifiManager.addParameter(&custom_rgborder);

    wifiManager.addParameter(&custom_color1);
    wifiManager.addParameter(&custom_color2);
    wifiManager.addParameter(&custom_color3);
    wifiManager.addParameter(&custom_color4);
    wifiManager.addParameter(&custom_color5);
    wifiManager.addParameter(&custom_color6);
    wifiManager.addParameter(&custom_color7);
    wifiManager.addParameter(&custom_color8);
    wifiManager.addParameter(&custom_color9);
    wifiManager.addParameter(&custom_color10);

    wifiManager.addParameter(&custom_text_dhcp);
    wifiManager.addParameter(&custom_ip);
    wifiManager.addParameter(&custom_netmask);
    wifiManager.addParameter(&custom_gw);

    String Hostname = "WemosD1-" + WiFi.macAddress();

    wifiManager.setConfigPortalTimeout(configPortalTimeout);

    if (startWifiManager == true) {
      digitalWrite(LED_BUILTIN, LOW);
      if (!wifiManager.startConfigPortal(Hostname.c_str())) {
        Serial.println("failed to connect and hit timeout");
        delay(1000);
        ESP.restart();
      }
    }

    wifiManager.setSTAStaticIPConfig(IPAddress(ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]), IPAddress(gwBytes[0], gwBytes[1], gwBytes[2], gwBytes[3]), IPAddress(netmaskBytes[0], netmaskBytes[1], netmaskBytes[2], netmaskBytes[3]));

    wifiManager.autoConnect(Hostname.c_str());

    Serial.println("Wifi Connected");
    Serial.println("CUSTOM STATIC IP: " + String(ip) + " Netmask: " + String(netmask) + " GW: " + String(gw));
    if (shouldSaveConfig) {
      if (String(custom_ip.getValue()).length() > 5) {
        Serial.println("Custom IP Address is set!");
        strcpy(ip, custom_ip.getValue());
        strcpy(netmask, custom_netmask.getValue());
        strcpy(gw, custom_gw.getValue());
      } else {
        strcpy(ip,      "0.0.0.0");
        strcpy(netmask, "0.0.0.0");
        strcpy(gw,      "0.0.0.0");
      }
      strcpy(GlobalConfig.ccuIp, custom_ccuip.getValue());
      strcpy(GlobalConfig.deviceName, custom_devicename.getValue());
      GlobalConfig.NumLeds = atoi(custom_numleds.getValue());
      GlobalConfig.SelectedEOrder = atoi(custom_rgborder.getValue());
      GlobalConfig.restoreStateFromCCU = (atoi(custom_cbrestorestate.getValue()) == 1);

      Dimmer2ColorDefinition[0] = String(custom_color1.getValue());
      Dimmer2ColorDefinition[1] = String(custom_color2.getValue());
      Dimmer2ColorDefinition[2] = String(custom_color3.getValue());
      Dimmer2ColorDefinition[3] = String(custom_color4.getValue());
      Dimmer2ColorDefinition[4] = String(custom_color5.getValue());
      Dimmer2ColorDefinition[5] = String(custom_color6.getValue());
      Dimmer2ColorDefinition[6] = String(custom_color7.getValue());
      Dimmer2ColorDefinition[7] = String(custom_color8.getValue());
      Dimmer2ColorDefinition[8] = String(custom_color9.getValue());
      Dimmer2ColorDefinition[9] = String(custom_color10.getValue());

      saveSystemConfig();

      delay(100);
      ESP.restart();
    }
    return true;
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("AP-Modus ist aktiv!");
}

void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    bytes[i] = strtoul(str, NULL, base);
    str = strchr(str, sep);
    if (str == NULL || *str == '\0') {
      break;
    }
    str++;
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
