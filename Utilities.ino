int x2i(char *s) {
  int x = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
    }
    else break;
    s++;
  }
  return x;
}

int dim2val(int dimVal) {
  int hex = 0;
  char hexArray[HEXARRAY_SIZE];
  if (dimVal > 0) {
    //DEBUG("requested color value = " + String(Dimmer2ColorDefinition[dimVal - 1]));
    String hexValue = Dimmer2ColorDefinition[dimVal - 1];
    hexValue.toCharArray(hexArray, HEXARRAY_SIZE);
    hex = x2i(hexArray);
  } else {
    hex = x2i("000000");
  }
  return hex;
}

String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]); 
}
