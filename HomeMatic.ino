void getValuesFromCCU() {
  String ChannelName =  "CUxD." + getStateCUxD(GlobalConfig.deviceName, "Address");
  Serial.println("ChannelName = " + ChannelName);
  Serial.println("\ngetValuesFromCCU: ");

  for (int i = 1; i < 17; i++) {
    String val = getStateCUxD(ChannelName + ":" + String(i) + ".LEVEL", "State");
    if (val != "null") {
      float a = val.toFloat();
      int dimVal = a * 10;
      Serial.print("dimVal = "+String(dimVal) + "; ");
      int color = dim2val(dimVal);
      setLed(i - 1, color );
    }
  }
  Serial.println();
}

String getStateCUxD(String id, String type) {
  if (id.indexOf(".null.") == -1) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.setTimeout(2500);
      id.replace(" ", "%20");
      String url = "http://" + String(GlobalConfig.ccuIp) + ":8181/cuxd.exe?ret=dom.GetObject(%22" + id + "%22)." + type + "()";
      // Serial.print("getStateFromCUxD url: " + url + " -> ");
      http.begin(url);
      int httpCode = http.GET();
      String payload = "error";
      if (httpCode > 0) {
        payload = http.getString();
      }
      if (httpCode != 200) {
        Serial.println("HTTP " + id + " fail");
      }
      http.end();

      payload = payload.substring(payload.indexOf("<ret>"));
      payload = payload.substring(5, payload.indexOf("</ret>"));
      //Serial.println("result: " + payload);

      return payload;
    } else {
      if (!doWifiConnect())
        ESP.restart();
    }
  } else return "";
}

