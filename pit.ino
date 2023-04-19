
void handleOpenPit() {
  setPitState(Opening);

  
  enablePitSolenoid();


  t.setTimeout([=]() {
    fowardPitMotor();
    t.setTimeout([=]() {
      disablePitSolenoid();
      stopPitMotor();
      setPitState(Open);
    },
                 PIT_RELAY_ENABLE_DURATION);
  },
               250);
}

void handlePitManualClose() {
  // setPitState(Closing);
  reversePitMotor();
}

void handlePitManualOpen() {
  // setPitState(Opening);
  fowardPitMotor();
}

void enablePitSolenoid() {
  digitalWrite(PIN_RELAY_SOLENOID, HIGH);
}

void disablePitSolenoid() {
  digitalWrite(PIN_RELAY_SOLENOID, LOW);
}

void fowardPitMotor() {
  digitalWrite(PIN_PIT_OPEN_LED, HIGH);
  pitMotor.writeMicroseconds(PIT_MOTOR_PWM_FWD);
}

void reversePitMotor() {
  digitalWrite(PIN_PIT_CLOSE_LED, HIGH);
  pitMotor.writeMicroseconds(PIT_MOTOR_PWM_REV);
}

void stopPitMotor() {
  digitalWrite(PIN_PIT_OPEN_LED, LOW);
  digitalWrite(PIN_PIT_CLOSE_LED, LOW);
  pitMotor.writeMicroseconds(PIT_MOTOR_PWM_STOP);
}

void setPitState(PitState s) {
  pitState = s;
}