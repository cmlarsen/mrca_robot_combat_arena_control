// ########## Remote Timer Pod


void updateTimerDisplay() {
  Serial.print("Clock Display:");

  int displayTime = formatTimerDuration(remainingTime);
  Serial.println(displayTime);
  clockDisplay.drawColon(true);
  clockDisplay.print(displayTime, DEC);
  clockDisplay.writeDisplay();
  // ########## Remote Timer Pod
  remoteData.timerDisplayValue = displayTime;
}
