bool loadSystemConfig() {
  DEBUG("mounting FS...","loadSystemConfig()",_slInformational);
  if (SPIFFS.begin()) {
    DEBUG("mounted file system","loadSystemConfig()",_slInformational);
    if (SPIFFS.exists("/" + configJsonFile)) {
      DEBUG("reading config file","loadSystemConfig()",_slInformational);
      File configFile = SPIFFS.open("/" + configJsonFile, "r");
      if (configFile) {
        DEBUG("opened config file","loadSystemConfig()",_slInformational);
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          DEBUG("parsed json","loadSystemConfig()",_slInformational);
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
          DEBUG("failed to load json config","loadSystemConfig()",_slError);
        }
      }
      return true;
    } else {
      DEBUG("/" + configJsonFile + " not found.","loadSystemConfig()",_slWarning);
      return false;
    }
    SPIFFS.end();
  } else {
    DEBUG("failed to mount FS","loadSystemConfig()",_slError);
    return false;
  }
}

bool saveSystemConfig() {
  SPIFFS.begin();
  DEBUG("saving config","saveSystemConfig()",_slInformational);
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
    DEBUG("failed to open config file for writing","saveSystemConfig()",_slError);
  }

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();

  SPIFFS.end();
}
