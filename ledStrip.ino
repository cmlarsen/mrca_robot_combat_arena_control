
void blink(int duration, Color color) {
  //start color
  setLEDs(color);
  t.setTimeout([]() {
    setLEDs(Black);
  },
               duration);
}

void setLEDs(Color color) {
  // Color Codes: http://fastled.io/docs/3.1/group___pixeltypes.html#gaeb40a08b7cb90c1e21bd408261558b99
  switch (color) {
    case Red:
      Serial.println("red");
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    case Green:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      Serial.println("green");
      break;
    case Blue:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      Serial.println("Blue");
      break;
    case Yellow:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      // fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      Serial.println("yellow");
      break;
    case Black:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      Serial.println("black");
      break;
    case White:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      // fill_solid(leds, NUM_LEDS, CRGB::White);
      Serial.println("white");
      break;
  }
  FastLED.show();
}