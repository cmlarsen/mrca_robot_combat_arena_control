// ########## Sequences
//Run at startup, beep and blink a bit
void selfCheckSequence() {
  beep(150, BeepLow);
  delay(500);
  beep(150, BeepHigh);
  blink(1000, Red);
  blink(1000, Black);
  blink(1000, Yellow);
  blink(1000, Black);
  blink(1000, Green);
  blink(1000, Black);
  setLEDs(White);
}

// A blocking sequence of beeps and lights that plays before the match starts or resumes
int runMatchIntroSequence() {
  Serial.println("Match Intro Sequence Running - Beep, beep, beep, BEEEEEEP");
  for (int i = 0; i < 3; i++) {
    beepSync(BeepLow);
    setLEDs(Yellow);
    delay(200);

    beepSync(-1);
    setLEDs(Black);
    delay(800);
  }
  beep(1000, BeepHigh);
}

//A  NON Blocking sequence to run before we stop the match
void runPreStopSequence() {
  Serial.println("Match Pre Stop Sequence");
}

