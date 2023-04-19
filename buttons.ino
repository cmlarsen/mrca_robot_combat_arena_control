
// ########## Button Handlers
void handleStartButton() {
  if (buttonPressed(Start) && !buttonPressed(Pause)) {
    Serial.println("Start");
    if (!matchTimerLoopRunning() && matchState == Ready) {
      startingMatch();
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
      readyMatch();
    }
  }
}

bool wasPressedPitSolenoid = false;
void handlePitManualSolenoidButton() {
  if (buttonPressed(PitSolenoid)) {
    Serial.println("PitSolenoid");
    enablePitSolenoid();
    wasPressedPitSolenoid = true;
  } else {
    if (wasPressedPitSolenoid) {
      disablePitSolenoid();
      wasPressedPitSolenoid = false;
    }
  }
}


bool wasPressedPitOpen = false;
void handlePitManualOpenButton() {
  if (buttonPressed(PitOpen) && (!buttonPressed(PitClose))) {
    digitalWrite(PIN_PIT_OPEN_LED, HIGH);
    Serial.println("PitOpen");
    enablePitSolenoid();
    t.setTimeout([]() {
      handlePitManualOpen();
      wasPressedPitOpen = true;
    },
                 100);
  } else {
    if (wasPressedPitOpen) {
      digitalWrite(PIN_PIT_OPEN_LED, LOW);
      wasPressedPitOpen = false;
      disablePitSolenoid();
      stopPitMotor();
    }
  }
}

bool wasPressedPitClose = false;
void handlePitManualCloseButton() {
  if (buttonPressed(PitClose) && !buttonPressed(PitOpen)) {
    Serial.println("PitClose");
    handlePitManualClose();
    wasPressedPitClose = true;
  } else {
    if (wasPressedPitClose) {
      digitalWrite(PIN_PIT_CLOSE_LED, LOW);
      stopPitMotor();
      wasPressedPitClose = false;
    }
  }
}

void tryStopMotor() {

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
          addedTime += ADDED_TIME_DURATION;
          cumlativeAddedTime +=ADDED_TIME_DURATION;
          addTimeButtonTimeout = 0;
        },
        100);
    }
  }
}

void handlePitDisableButton() {
  pitEnabled = digitalRead(PIN_PIT_DISABLE) == LOW;
}


bool buttonPressed(Button button) {
  switch (button) {
    case Start:
      // Serial.println("Btn Start");
      return digitalRead(PIN_BUTTON_START) == HIGH;
    case Pause:
      // Serial.println("Btn Pause");
      return digitalRead(PIN_BUTTON_PAUSE) == HIGH;
    case Stop:
      // Serial.println("Btn Stop");
      return digitalRead(PIN_BUTTON_STOP) == HIGH;
    case AddTime:
      // Serial.println("Btn AddTime");
      return digitalRead(PIN_BUTTON_ADD_TIME) == HIGH;
    case PitOpen:
      // Serial.println("Btn PitOpen");
      return digitalRead(PIN_PIT_OPEN) == HIGH;
    case PitClose:
      // Serial.println("Btn PitClose");
      return digitalRead(PIN_PIT_CLOSE) == HIGH;
    case PitSolenoid:
      // Serial.println("Btn PitSolenoid");
      return digitalRead(PIN_PIT_SOLENOID_ENABLE) == HIGH;
  }
}
