
void handleOpenPit() {
  setPitState(Opening);
  digitalWrite(PIN_RELAY_SOLENOID, HIGH);
  
  t.setTimeout([=]() {
    digitalWrite(PIN_RELAY_SOLENOID, LOW);
    setPitState(Open);
  },
               PIT_RELAY_ENABLE_DURATION);
}

void handlePitClose() {
  setPitState(Closing);
  Serial.println("Close Pit");
}

void setPitState(PitState s) {
  pitState = s;
}