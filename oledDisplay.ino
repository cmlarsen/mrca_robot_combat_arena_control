
void updateOLED() {
  display.clearDisplay();
  // Display Time
  display.setTextSize(1);
  display.setCursor(0, 8);
  int h = floor(remainingTime / 60);
  int m = remainingTime % 60;
  display.print("Time ");
  display.print(h);
  display.print(":");
  if (m < 10) {
    display.print(0);
  }
  display.print(m);
  display.setCursor(65, 8);
  display.print("+");
  int cM = floor(cumlativeAddedTime / 60);
  int cS = cumlativeAddedTime % 60;
  display.print(cM);
  display.print(":");
  if (cS < 10) {
    display.print(0);
  }
  display.print(cS);

  // Display Match State
  display.setTextSize(2);
  display.setCursor(0, 16);
  switch (matchState) {
    case Starting:
      display.print("Starting");
      break;
    case Running:
      display.print("Running");
      break;
    case Paused:
      display.print("Paused");
      break;
    case Stopped:
      display.print("Stopped");
      break;
    case Ready:
      display.print("Ready");
      break;
  }

  // Display Pit State
  display.setCursor(0, 32);
  display.print("Pit ");
  display.println(pitEnabled ? "on" : "off");
  switch (pitState) {
    case Open:
      display.println("Open");
      break;
    case Opening:
      display.println("Opening");
      break;
    case Closing:
      display.println("Closing");
      break;
    case Closed:
      display.println("Closed");
      break;
  }


  display.display();
}
