
// ########## Button Handlers
void handleStartButton() {
  if (buttonPressed(Start) && !buttonPressed(Pause)) {
    if (!matchTimerLoopRunning() && matchState == Ready) {
      startMatch();
    } else if (matchState == Paused) {
      resumeMatch();
    }
  }
}

void handlePauseButton() {
  if (buttonPressed(Pause) && !buttonPressed(Start)) {
    if (matchTimerLoopRunning() && matchState == Running) {
      pauseMatch();
    }
  }
}

void handleStopButton() {
  if (buttonPressed(Stop)) {
    if (matchTimerLoopRunning()) {
      stopMatch();
    } else {
      //  Reset the system state
    }
  }
}

void handlePitManualSolenoidButton() {
  if (buttonPressed(PitSolenoid)) {
    Serial.println("handlePitManualSolenoidButton HIGH");
    enablePitSolenoid();
  } else {
    if (pitState != Opening) {
      disablePitSolenoid();
    }
  }
}

void handlePitManualOpenButton() {
  if (buttonPressed(PitOpen) && (!buttonPressed(PitClose))) {
    Serial.println("handlePitManualOpenButton HIGH");
    handlePitManualOpen();
  }
}

void handlePitManualCloseButton() {
  if (buttonPressed(PitClose) && !buttonPressed(PitOpen)) {
      Serial.println("handlePitManualCloseButton HIGH");
      handlePitManualClose();
    }
}

void tryStopMotor() {
  if (!buttonPressed(PitOpen) && !buttonPressed(PitClose) && pitState != Opening && pitState != Closing) {
    stopPitMotor();
  }
}

void handleAddTimeButton() {
  if (buttonPressed(AddTime)) {
    Serial.print("handleAddTimeButton");
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
