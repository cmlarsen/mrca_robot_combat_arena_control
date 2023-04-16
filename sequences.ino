// ########## Sequences
//Run at startup, beep and blink a bit
void selfCheckSequence() {
  beep(150);
  delay(500);
  beep(150);
  blink(1000, Red);
  blink(1000, Black);
  blink(1000, Yellow);
  blink(1000, Black);
  blink(1000, Green);
  blink(1000, Black);
  setLEDs(White);
}

// A sequence of beeps and lights that plays before the match starts or resumes
// TODO this might not work as expected
int runMatchIntroSequence() {
  Serial.println("Match Intro Sequence Running - Beep, beep, beep, BEEEEEEP");
  for (int i = 0; i < 3; i++) {
    beep(200);
    setLEDs(Yellow);
    delay(200);

    
    setLEDs(Black);
    delay(800);
  }
  beep(1000);
}

//A  NON Blocking sequence to run before we stop the match
void runPreStopSequence() {
  Serial.println("Match Pre Stop Sequence");
}

