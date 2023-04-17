
// ########## Match State
void startingMatch() {
  elapsedTime = COUNTDOWN_DURATION;
  remainingTime = COUNTDOWN_DURATION;
  Serial.println("Starting Match");
  runStartingSequence();
  matchState = Starting;
}

void startMatch() {
  beep(1000);
  Serial.println("Running Match");
  setLEDs(Green);
  matchState = Running;
}

void pauseMatch() {
  Serial.println("pauseMatch");
  beep(1000);
  setLEDs(Yellow);
  matchState = Paused;
}

void resumeMatch() {
  beep(1000);
  // startingMatch();
  // Serial.println("resumeMatch");
  // runMatchIntroSequence();
  matchState = Running;
  setLEDs(Green);
}

void stopMatch() {
  Serial.println("stopMatch");
  matchState = Stopped;
  setLEDs(Red);
  t.cancel(matchIntervalId);
  beep(2000);
  // Turn LEDs White after a while
  t.setTimeout(readyMatch, 5000);
}

void readyMatch() {
  matchIntervalId = 0;
  totalMatchDuration = MATCH_DURATION;
  addedTime = 0;
  elapsedTime = 0;
  matchState = Ready;
  setLEDs(White);
  remainingTime = MATCH_DURATION;
  // updateTimerDisplay();
  pitState = Closed;
}
