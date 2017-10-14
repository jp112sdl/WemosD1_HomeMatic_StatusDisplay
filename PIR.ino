void handlePIRInterrupt() {
  PIRInterruptDetected = (digitalRead(PIR_PIN) == HIGH) ? 1 : 2;
}

void initPIR() {
  pinMode(PIR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), handlePIRInterrupt, CHANGE);
}

void handlePIR() {
  if (PIRInterruptDetected) {
    switch (PIRInterruptDetected) {
      case 1:
        DEBUG("PIR Interrupt HIGH detected!", "handlePIR()", _slInformational);
        setLedMode(Wake, 0);
        break;
      case 2:
        DEBUG("PIR Interrupt LOW detected!", "handlePIR()", _slInformational);
        setLedMode(Sleep);
        break;
    }
    PIRInterruptDetected = 0;
  }
}


