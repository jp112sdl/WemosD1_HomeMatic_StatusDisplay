bool doWifiConnect() {
  String _ssid = WiFi.SSID();
  String _psk = WiFi.psk();

  const char* ipStr = ip; byte ipBytes[4]; parseBytes(ipStr, '.', ipBytes, 4, 10);
  const char* netmaskStr = netmask; byte netmaskBytes[4]; parseBytes(netmaskStr, '.', netmaskBytes, 4, 10);
  const char* gwStr = gw; byte gwBytes[4]; parseBytes(gwStr, '.', gwBytes, 4, 10);
  if (!startWifiManager && _ssid != "" && _psk != "" ) {
    DEBUG(F("Connecting WLAN the classic way..."));
#ifdef SERIALDEBUG && WM_DEBUG_OUTPUT
    DEBUG("Connecting to SSID: " + _ssid); //+" with KEY: "+_psk);
#endif
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.hostname(GlobalConfig.DeviceName);
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
    DEBUG("Wifi Connected", "doWifiConnect()", _slInformational);
    return true;
  } else {
    WiFiManager wifiManager;
#ifdef SERIALDEBUG && WM_DEBUG_OUTPUT
    wifiManager.setDebugOutput(true);
#endif
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    WiFiManagerParameter custom_text_hm("<div>HomeMatic Konfiguration:</div>");
    WiFiManagerParameter custom_ccuip("ccuip", "IP der CCU2", GlobalConfig.CcuIp, IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_devicename("devicename", "Ger&auml;tename", GlobalConfig.DeviceName, VARIABLE_SIZE, INPUT_TEXT, "pattern='\\w{1,}'");
    char*chrRestoreOldState = "0";
    if (GlobalConfig.RestoreStateFromCCU) chrRestoreOldState =  "1" ;
    WiFiManagerParameter custom_cbrestorestate("restorestate", "Boot: Lade Werte von CCU", chrRestoreOldState, 8, INPUT_CHECKBOX);

    char*chrPIRtoCCU = "0";
    if (GlobalConfig.PIRtoCCU) chrPIRtoCCU =  "1" ;
    WiFiManagerParameter custom_cbpirtoccu("pirtoccu", "BWM an CCU senden", chrPIRtoCCU, 8, INPUT_CHECKBOX);


    WiFiManagerParameter custom_text_led("<div>LED Konfiguration:</div>");
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

    WiFiManagerParameter custom_text_dimmerconfig("<div>CUxD Dimmer-Konfiguration:</div>");
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

    WiFiManagerParameter custom_text_dimmerblink("<div>Blinken f&uuml;r Dimmer-Werte ab...<br/>(11 = kein blinken)</div>");
    WiFiManagerParameter custom_dimblink("dimblink", "Blinken f&uuml;r Dimmer-Werte ab...", String(GlobalConfig.DimBlink).c_str(), VARIABLE_SIZE, INPUT_TEXT, "required pattern='[0-9]{1,2}'");

    WiFiManagerParameter custom_text_dhcp("<div>Statische IP (wenn leer, dann DHCP):</div>");
    WiFiManagerParameter custom_ip("custom_ip", "IP-Adresse", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_netmask("custom_netmask", "Netzmaske", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");
    WiFiManagerParameter custom_gw("custom_gw", "Gateway", "", IP_SIZE, INPUT_TEXT, "pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$'");

    wifiManager.addParameter(&custom_text_led);
    wifiManager.addParameter(&custom_numleds);
    wifiManager.addParameter(&custom_rgborder);

    wifiManager.addParameter(&custom_text_hm);
    wifiManager.addParameter(&custom_ccuip);
    wifiManager.addParameter(&custom_devicename);
    wifiManager.addParameter(&custom_cbrestorestate);
    wifiManager.addParameter(&custom_cbpirtoccu);

    wifiManager.addParameter(&custom_text_dimmerconfig);
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

    wifiManager.addParameter(&custom_text_dimmerblink);
    wifiManager.addParameter(&custom_dimblink);

    wifiManager.addParameter(&custom_text_dhcp);
    wifiManager.addParameter(&custom_ip);
    wifiManager.addParameter(&custom_netmask);
    wifiManager.addParameter(&custom_gw);

    String Hostname = "WemosD1-" + WiFi.macAddress();

    wifiManager.setConfigPortalTimeout(configPortalTimeout);

    if (startWifiManager == true) {
      digitalWrite(LED_BUILTIN, LOW);
      if (!wifiManager.startConfigPortal(Hostname.c_str())) {
        DEBUG("failed to connect and hit timeout");
        delay(1000);
        ESP.restart();
      }
    }

    wifiManager.setSTAStaticIPConfig(IPAddress(ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]), IPAddress(gwBytes[0], gwBytes[1], gwBytes[2], gwBytes[3]), IPAddress(netmaskBytes[0], netmaskBytes[1], netmaskBytes[2], netmaskBytes[3]));

    wifiManager.autoConnect(Hostname.c_str());

    DEBUG("Wifi Connected");
    DEBUG("CUSTOM STATIC IP: " + String(ip) + " Netmask: " + String(netmask) + " GW: " + String(gw));
    if (shouldSaveConfig) {
      if (String(custom_ip.getValue()).length() > 5) {
        DEBUG("Custom IP Address is set!");
        strcpy(ip, custom_ip.getValue());
        strcpy(netmask, custom_netmask.getValue());
        strcpy(gw, custom_gw.getValue());
      } else {
        strcpy(ip,      "0.0.0.0");
        strcpy(netmask, "0.0.0.0");
        strcpy(gw,      "0.0.0.0");
      }
      strcpy(GlobalConfig.CcuIp, custom_ccuip.getValue());
      strcpy(GlobalConfig.DeviceName, custom_devicename.getValue());
      GlobalConfig.NumLeds = atoi(custom_numleds.getValue());
      GlobalConfig.SelectedEOrder = atoi(custom_rgborder.getValue());
      GlobalConfig.RestoreStateFromCCU = (atoi(custom_cbrestorestate.getValue()) == 1);
      GlobalConfig.PIRtoCCU = (atoi(custom_cbpirtoccu.getValue()) == 1);

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

      GlobalConfig.DimBlink = atoi(custom_dimblink.getValue());

      saveSystemConfig();

      delay(100);
      ESP.restart();
    }
    return true;
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  DEBUG("AP-Modus ist aktiv!");
}

void saveConfigCallback () {
  DEBUG("Should save config");
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
  DEBUG("SSID: " + WiFi.SSID());
  DEBUG("IP Address: " + IpAddress2String(WiFi.localIP()));
  DEBUG("Gateway Address: " + IpAddress2String(WiFi.gatewayIP()));
  DEBUG("signal strength (RSSI):" + String(WiFi.RSSI()) + " dBm");
}
