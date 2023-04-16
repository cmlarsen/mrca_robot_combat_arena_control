
// ########## Helpers
//Is the match timer loop Running
bool matchTimerLoopRunning() {
  return matchIntervalId != 0;
}

int formatTimerDuration(int duration) {
  int minutes = floor(duration / 60);
  int seconds = duration % 60;
  // Display time "2 min 34 sec" which is 154 seconds as "234" for the 7 segment display
  int displayTime = minutes * 100 + seconds;
  return displayTime;
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
