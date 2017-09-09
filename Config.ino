bool loadSystemConfig() {
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/" + bootConfigJsonFile)) {
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/" + bootConfigJsonFile, "r");
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
          ((json["ccuip"]).as<String>()).toCharArray(GlobalConfig.ccuIp, IP_SIZE);
          ((json["devicename"]).as<String>()).toCharArray(GlobalConfig.deviceName, VARIABLE_SIZE);
          
          GlobalConfig.restoreStateFromCCU = json["restorestate"];

          for (int i = 0; i < COLOR_COUNT; i++) {
            String colorStr = (json["color" + String(i)]).as<String>();
            if (colorStr != "") 
            Dimmer2ColorDefinition[i] = colorStr;
          }

          GlobalConfig.NumLeds = (json["numleds"]).as<int>();
          if (GlobalConfig.NumLeds == 0)
            GlobalConfig.NumLeds = 1;

          GlobalConfig.SelectedEOrder = (json["eorder"]).as<int>();
          GlobalConfig.LedBrightness = (json["brightness"]).as<byte>();
          if (GlobalConfig.LedBrightness < 5) GlobalConfig.LedBrightness = 255;
        } else {
          Serial.println("failed to load json config");
        }
      }
      return true;
    } else {
      Serial.println("/" + bootConfigJsonFile + " not found.");
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
  json["ccuip"] = GlobalConfig.ccuIp;
  json["devicename"] = GlobalConfig.deviceName;
  json["numleds"] = GlobalConfig.NumLeds;
  json["eorder"] = GlobalConfig.SelectedEOrder;
  json["brightness"] = GlobalConfig.LedBrightness;
  json["restorestate"] = GlobalConfig.restoreStateFromCCU;

  for (int i = 0; i < COLOR_COUNT; i++) {
    Dimmer2ColorDefinition[i].toUpperCase();
    json["color" + String(i)] = Dimmer2ColorDefinition[i];
  }

  SPIFFS.remove("/" + bootConfigJsonFile);
  File configFile = SPIFFS.open("/" + bootConfigJsonFile, "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }

  json.printTo(Serial);
  Serial.println("");
  json.printTo(configFile);
  configFile.close();

  SPIFFS.end();
}
