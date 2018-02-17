void getValuesFromCCU() {
  if (String(GlobalConfig.CcuIp) != "" && String(GlobalConfig.CcuIp) != "0.0.0.0") {
    String ChannelName =  "CUxD." + getStateCUxD(GlobalConfig.DeviceName, "Address");
    DEBUG("ChannelName = " + ChannelName, "getValuesFromCCU()", _slInformational);

    String _DebugText = "";
    byte _maxChannels = GlobalConfig.NumLeds + 1;
    if (GlobalConfig.NumLeds > 16)
      _maxChannels = 17;

    for (int i = 1; i < _maxChannels; i++) {
      String val = getStateCUxD(ChannelName + ":" + String(i) + ".LEVEL", "State");
      if (val != "null") {
        float a = val.toFloat();
        int dimVal = a * 10;
        _DebugText += "dimVal = " + String(dimVal) + "; ";
        int color = dim2val(dimVal);
        int ledNum = i - 1;
        LEDConfig.Blink[ledNum] = (dimVal >= GlobalConfig.DimBlink);
        setLed(ledNum, color);
      }
    }
    DEBUG("getValuesFromCCU: " + _DebugText, "getValuesFromCCU()", _slInformational);
  } else {
    DEBUG("CCU IP not set!", "getValuesFromCCU()", _slInformational);
  }
}

String getStateCUxD(String id, String type) {
  if (id.indexOf(".null.") == -1) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.setTimeout(HTTPGETTIMEOUT);
      id.replace(" ", "%20");
      String url = "http://" + String(GlobalConfig.CcuIp) + ":8181/cuxd.exe?ret=dom.GetObject(%22" + id + "%22)." + type + "()";
      DEBUG("getStateFromCUxD url: " + url + " -> ", "getStateCUxD()", _slInformational);
      http.begin(url);
      int httpCode = http.GET();
      String payload = "error";
      if (httpCode > 0) {
        payload = http.getString();
      }
      if (httpCode != 200) {
        DEBUG("HTTP " + id + " fail", "getStateCUxD()", _slError);
      }
      http.end();

      payload = payload.substring(payload.indexOf("<ret>"));
      payload = payload.substring(5, payload.indexOf("</ret>"));
      DEBUG("result: " + payload, "getStateCUxD()", _slInformational);

      return payload;
    } else {
      if (!doWifiConnect())
        ESP.restart();
    }
  } else return "";
}

bool setStateCUxD(String id, String value) {
  if (id.indexOf(".null.") == -1) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.setTimeout(HTTPGETTIMEOUT);
      id.replace(" ", "%20");
      String url = "http://" + String(GlobalConfig.CcuIp) + ":8181/cuxd.exe?ret=dom.GetObject(%22" + id + "%22).State(" + value + ")";
      DEBUG("setStateCUxD url: " + url, "setStateCUxD()", _slInformational);
      http.begin(url);
      int httpCode = http.GET();
      String payload = "";

      if (httpCode > 0) {
        DEBUG("HTTP " + id + " success", "setStateCUxD()", _slInformational);
        payload = http.getString();
      }
      if (httpCode != 200) {
        DEBUG("HTTP " + id + " failed with HTTP Error Code " + String(httpCode), "setStateCUxD()", _slError);
      }
      http.end();

      payload = payload.substring(payload.indexOf("<ret>"));
      payload = payload.substring(5, payload.indexOf("</ret>"));


      DEBUG("result: " + payload, "setStateCUxD()", (payload != "null") ? _slInformational : _slError);

      return (payload != "null");

    } else {
      if (!doWifiConnect())
        ESP.restart();
    }
  } else return true;
}

