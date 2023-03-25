
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

void handlePitOpenButton() {
  if (buttonPressed(PitOpen)) {
    Serial.println("handlePitOpenButton HIGH");
    handleOpenPit();
  } else {
    if (pitState != Opening) {
      digitalWrite(PIN_RELAY_SOLENOID, LOW);
    }
  }
}
void handlePitCloseButton() {
  if (buttonPressed(PitClose)) {
    Serial.println("handlePitCloseButton HIGH");
    handlePitClose();
  } else {
    if (pitState != Opening) {
      digitalWrite(PIN_RELAY_SOLENOID, LOW);
    }
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