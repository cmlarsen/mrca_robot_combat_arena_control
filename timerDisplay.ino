// ########## Remote Timer Pod


void updateTimerDisplay(int time) {

  int displayTime = formatTimerDuration(time);
  // Serial.print("Clock Display:");
  // Serial.println(displayTime);
  clockDisplay.drawColon(true);
  clockDisplay.print(displayTime, DEC);
  clockDisplay.writeDisplay();
  // ########## Remote Timer Pod
  remoteData.timerDisplayValue = displayTime;
}
