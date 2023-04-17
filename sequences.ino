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




// void introLoop(){
// count++
// updateTimerDisplay();
// }
// A sequence of beeps and lights that plays before the match starts or resumes
// TODO this might not work as expected
int runMatchIntroSequence() {
  Serial.println("Match Intro Sequence Running - Beep, beep, beep, BEEEEEEP");
  for (int i = 0; i < 3; i++) {
    // updateTimerDisplay(3 - i);
    beep(500);
    setLEDs(Yellow);
    delay(500);
    // updateTimerDisplay(3-i);
    setLEDs(Black);
    delay(500);
  }
  
}

void runStartingSequence() {
  Serial.println("Match Starting Sequence");

  for (int i = 0; i < STOP_SEQUENCE_DURATION; i++) {
    t.setTimeout(blinkYellow,
                 1000 * i);
    t.setTimeout(blinkBlack,
                 1000 * i + 500);
  }
}

void blinkYellow() {
  beep(500);
  setLEDs(Yellow);
}
void blinkGreen() {
  setLEDs(Green);
}
void blinkBlack() {
  setLEDs(Black);
}
//A  NON Blocking sequence to run before we stop the match
void runPreStopSequence() {
  Serial.println("Match Pre Stop Sequence");

  for (int i = 0; i < STOP_SEQUENCE_DURATION; i++) {
    t.setTimeout(blinkYellow,
                 1000 * i);
    t.setTimeout(blinkGreen,
                 1000 * i + 500);
  }
}
