void initUDPClient() {
  UDPClient.UDP.begin(UDPPORT);
  DEBUG("Starte UDP Client an Port " + String(UDPPORT)+"...");
  UDPReady = true;
}

void handleUDP() {
  String udpMessage = receivedUDP();
  if (udpMessage == "sleep")
    setLedMode(Sleep);
    
  if (udpMessage == "wakeup")
    setLedMode(Wake);
    
  if (udpMessage.indexOf("wakeup?t=") != -1) {
    int _DisplayTimeoutSeconds = (udpMessage.substring(9, udpMessage.length())).toInt();
    setLedMode(Wake, _DisplayTimeoutSeconds);
  }

  if (udpMessage.indexOf("led=") != -1 && udpMessage.indexOf("hex=") != -1) {
    int led = (udpMessage.substring(4, udpMessage.indexOf("&hex="))).toInt();
    String hexValue = udpMessage.substring(udpMessage.indexOf("&hex=") + 5, udpMessage.length());
    char hexArray[16];
    hexValue.replace("#", "");
    hexValue.toUpperCase();
    hexValue.toCharArray(hexArray, HEXARRAY_SIZE);
    setLed(led, x2i(hexArray));
  }

  if (udpMessage.indexOf("brightness=") != -1) {
    byte brightness = (udpMessage.substring(11, udpMessage.length())).toInt();
    setLedBrightness(brightness);
  }
}

String receivedUDP() {
  int packetSize = UDPClient.UDP.parsePacket();
  if (packetSize) {
    DEBUG("Received "+String(packetSize)+" bytes from "+UDPClient.UDP.remoteIP().toString()+", port "+String(UDPClient.UDP.remotePort()));
    int len = UDPClient.UDP.read(UDPClient.incomingPacket, 255);
    if (len > 0)
      UDPClient.incomingPacket[len] = 0;

    DEBUG("UDP packet contents: " + String(UDPClient.incomingPacket));

    UDPClient.UDP.beginPacket(UDPClient.UDP.remoteIP(), UDPClient.UDP.remotePort());
    UDPClient.UDP.write("<ACK>");
    UDPClient.UDP.endPacket();

    String message = String(UDPClient.incomingPacket);
    message.trim();
    return message;
  } else return "";
}
