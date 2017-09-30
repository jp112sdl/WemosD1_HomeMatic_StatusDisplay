bool loadSystemConfig() {
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/" + configJsonFile)) {
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/" + configJsonFile, "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        Serial.println("");
        if (json.success()) {
          Serial.println("\nparsed json");
          ((json["ip"]).as<String>()).toCharArray(ip, IP_SIZE);
          ((json["netmask"]).as<String>()).toCharArray(netmask, IP_SIZE);
          ((json["gw"]).as<String>()).toCharArray(gw, IP_SIZE);
          ((json["ccuip"]).as<String>()).toCharArray(GlobalConfig.CcuIp, IP_SIZE);
          ((json["devicename"]).as<String>()).toCharArray(GlobalConfig.DeviceName, VARIABLE_SIZE);

          GlobalConfig.RestoreStateFromCCU = json["restorestate"];

          for (int i = 0; i < COLOR_COUNT; i++) {
            String colorStr = (json["color" + String(i)]).as<String>();
            if (colorStr != "")
              Dimmer2ColorDefinition[i] = colorStr;
          }

          GlobalConfig.NumLeds = (json["numleds"]).as<int>();
          if (GlobalConfig.NumLeds == 0)
            GlobalConfig.NumLeds = 1;

          GlobalConfig.DimBlink = (json["dimBlink"]).as<byte>();
          if (GlobalConfig.DimBlink == 0) GlobalConfig.DimBlink = 11;

          GlobalConfig.SelectedEOrder = (json["eorder"]).as<int>();
          GlobalConfig.LedBrightness = (json["brightness"]).as<byte>();
          if (GlobalConfig.LedBrightness < 5) GlobalConfig.LedBrightness = 255;
        } else {
          Serial.println("failed to load json config");
        }
      }
      return true;
    } else {
      Serial.println("/" + configJsonFile + " not found.");
      return false;
    }
    SPIFFS.end();
  } else {
    Serial.println("failed to mount FS");
    return false;
  }
}

bool saveSystemConfig() {
  SPIFFS.begin();
  Serial.println("saving config");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ip"] = ip;
  json["netmask"] = netmask;
  json["gw"] = gw;
  json["ccuip"] = GlobalConfig.CcuIp;
  json["devicename"] = GlobalConfig.DeviceName;
  json["numleds"] = GlobalConfig.NumLeds;
  json["eorder"] = GlobalConfig.SelectedEOrder;
  json["brightness"] = GlobalConfig.LedBrightness;
  json["restorestate"] = GlobalConfig.RestoreStateFromCCU;
  json["dimBlink"] = GlobalConfig.DimBlink;

  for (int i = 0; i < COLOR_COUNT; i++) {
    Dimmer2ColorDefinition[i].toUpperCase();
    json["color" + String(i)] = Dimmer2ColorDefinition[i];
  }

  SPIFFS.remove("/" + configJsonFile);
  File configFile = SPIFFS.open("/" + configJsonFile, "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(Serial);
  Serial.println("");
  json.printTo(configFile);
  configFile.close();

  SPIFFS.end();
}
