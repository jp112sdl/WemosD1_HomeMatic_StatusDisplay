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
        Serial.println("PIR Interrupt HIGH detected!");
        setLedMode(Wake, 0);
        break;
      case 2:
        Serial.println("PIR Interrupt LOW detected!");
        setLedMode(Sleep);
        break;
    }
    PIRInterruptDetected = 0;
  }
}

