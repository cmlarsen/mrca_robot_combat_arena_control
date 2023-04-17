void beep(int duration) {
  remoteData.buzzerOn = true;
  t.setTimeout([]() {
    remoteData.buzzerOn = false;
  },
               duration);
}

// // Beeps, use with delay() for a blocking/synchronous beep
// void beepSync(int note) {
//   // TODO: Convert this to send a signal, move buzzer code to timer display
//   // if (note == -1) {
//   //   noTone(PIN_BUZZER);
//   // } else {
//   //   tone(PIN_BUZZER, note);
//   // }
// }
