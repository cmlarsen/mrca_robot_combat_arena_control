
// ########## Match State
void startMatch() {
  
  Serial.println("Starting Match");
  runMatchIntroSequence();
  //perhaps move this interval to the main loop to make it easier to see where the match is running
  // probrbaly in a thing like if matchState == running and we don't have a matchId, then start the interval
  // matchIntervalId = t.setInterval(matchLoop, 1000);
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
  Serial.println("resumeMatch");
  runMatchIntroSequence();
  matchState = Running;
  setLEDs(Green);
}

void stopMatch() {
  Serial.println("stopMatch");
  matchState = Stopped;
  setLEDs(Red);
  beep(2000);
  t.cancel(matchIntervalId);

  // Turn LEDs White after a while
  t.setTimeout(readyMatch, 5000);
  matchIntervalId = 0;
  totalMatchDuration = 0;
  addedTime = 0;
  elapsedTime = 0;
}

void readyMatch() {
  matchState = Ready;
  setLEDs(White);
  remainingTime = MATCH_DURATION;
  // updateTimerDisplay();
  pitState = Closed;
}

