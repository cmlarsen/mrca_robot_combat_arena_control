
// ########## Button Handlers
void handleStartButton() {
  if (buttonPressed(Start) && !buttonPressed(Pause)) {
    Serial.println("Start");
    if (!matchTimerLoopRunning() && matchState == Ready) {
      startMatch();
    } else if (matchState == Paused) {
      resumeMatch();
    }
  }
}

void handlePauseButton() {
  if (buttonPressed(Pause) && !buttonPressed(Start)) {
    Serial.println("Pause");
    if (matchTimerLoopRunning() && matchState == Running) {
      pauseMatch();
    }
  }
}

void handleStopButton() {
  if (buttonPressed(Stop)) {
    Serial.println("Stop");
    if (matchTimerLoopRunning()) {
      stopMatch();
    } else {
      //  Reset the system state
    }
  }
}

void handlePitManualSolenoidButton() {
  if (buttonPressed(PitSolenoid)) {
    Serial.println("PitSolenoid");
    
    enablePitSolenoid();
  } else {
    if (pitState != Opening) {
      disablePitSolenoid();
    }
  }
}

void handlePitManualOpenButton() {
  if (buttonPressed(PitOpen) && (!buttonPressed(PitClose))) {
    Serial.println("PitOpen");
    
    handlePitManualOpen();
  }
}

void handlePitManualCloseButton() {
  if (buttonPressed(PitClose) && !buttonPressed(PitOpen)) {
    Serial.println("PitClose");
    
    handlePitManualClose();
  }
}

void tryStopMotor() {
  Serial.print("!buttonPressed(PitOpen)");
  Serial.println(!buttonPressed(PitOpen));
  Serial.print("!buttonPressed(PitClose)");
  Serial.println(!buttonPressed(PitClose));
  if (buttonPressed(PitOpen) && buttonPressed(PitClose)) {
    
    stopPitMotor();
  }
}

void handleAddTimeButton() {
  if (buttonPressed(AddTime)) {

    Serial.println("AddTime");
    if (addTimeButtonTimeout != 0) {
      t.reset(addTimeButtonTimeout);
    } else {
      addTimeButtonTimeout = t.setTimeout(
        []() {
          Serial.println("Add Time Button pressed!");
          addedTime += 5;
          addTimeButtonTimeout = 0;
        },
        100);
    }
  }
}

void handlePitDisableButton() {
  pitEnabled = digitalRead(PIN_PIT_DISABLE) == LOW;
}
