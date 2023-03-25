
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
      return digitalRead(PIN_BUTTON_START) == LOW;
    case Pause:
      return digitalRead(PIN_BUTTON_PAUSE) == LOW;
    case Stop:
      return digitalRead(PIN_BUTTON_STOP) == LOW;
    case AddTime:
      return digitalRead(PIN_BUTTON_ADD_TIME) == LOW;
    case PitOpen:
      return digitalRead(PIN_PIT_OPEN) == LOW;
  }
}
