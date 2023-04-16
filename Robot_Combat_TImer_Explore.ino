#include <EasyTransfer.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <FastLED.h>
#include <Adafruit_SSD1306.h>
#include <AsyncTimer.h>
#include <Wire.h>
#include <Servo.h>
/**
AsyncTimer Docs: https://github.com/Aasim-A/AsyncTimer

  Arduino Nano Every pinout: https://www.etechnophiles.com/wp-content/uploads/2021/05/Arduino-Every-Pinout.png?ezimgfmt=ng:webp/ngcb40
  Arduino Nano Every details: https://www.etechnophiles.com/arduino-nano-every-pinout-specifications-schematic-datasheet/
  Fast LED API: http://fastled.io/docs/3.1/index.html
  Fast LED API Docs? : https://dmadison.github.io/FastLED/docss/index.html
  Adafruit LED Backpack Docs: https://github.com/adafruit/Adafruit_LED_Backpack


  INPUTS:
  Pit Power (Enable/Disable) Switch - disables the pit automatically opening.
  Open Pit - releases solenoid viarelay and enables pit motor via Relay?
  Start  -starts match
  Pause - pause match, press start to resume
  Stop - stops match;
  Add Time - adds 5 seconds per press;

  * These aren't connected to arduino
  Close Pit - Nothing, wired to the motors.
  Master Power - Disables power to system

  OUTPUTS:
  OLED
  Clocks
  Pit Solenoid Relay
  Buzzer

  TODO:
  - Use the easyTransfer TX/RX examples to send messages to slaves via RS485
  - Add button combo to set 3 min match
  - Make sure timer updates when time is added.
  - Add LEDs for Pit Open/Pit Close
**/


/**
Pin 
2 = RS485 Enable
3 = RGBLED data
4 = Disable pit
A3 = Closing LED
A2 = Opening LED

CAT5
Blue
Green
Blue Stripe
Green Stripe

**/



// PINS - Input
#define PIN_BUTTON_PAUSE 9
#define PIN_BUTTON_START 8
#define PIN_BUTTON_ADD_TIME 10
#define PIN_BUTTON_STOP 11
#define PIN_PIT_CLOSE 5
#define PIN_PIT_SOLENOID_ENABLE 6
#define PIN_PIT_OPEN 7
#define PIN_PIT_DISABLE 4



// PINS - Output
#define PIN_PIT_OPEN_LED 11
#define PIN_PIT_CLOSE_LED 12
#define PIN_RS485_ENABLE 2
#define PIN_RGB_LED_STRIP 13
#define PIN_MOTOR_ESC 15
#define PIN_RELAY_SOLENOID 14

// PINS - I2C
#define PIN_I2C_SDA 18
#define PIN_I2C_SCL 19

// System Config
#define MATCH_DURATION 10               //seconds
#define PIT_OPEN_AFTER_TIME 5           //seconds into the match that the pit will open
#define STOP_SEQUENCE_DURATION 3        //seconds
#define ADDED_TIME_DURATION 3           //seconds
#define NUM_LEDS 20                     //How many LEDs are there in the strip?
#define PIT_RELAY_ENABLE_DURATION 3000  //milliseconds - how long to enable the pit relay when it automatically triggers

// OLED Size
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// I2C Addresses
#define OLED_ADDRESS 0x3C             //console OLED
#define CLOCK_DISPLAY_1_ADDRESS 0x70  //EO Side timer display

// Notes
#define BeepHigh 740  //F#
#define BeepLow 554   //C#

// State Enums
enum MatchState {
  Ready,
  Running,
  Paused,
  Stopped
};

enum Button {
  Start,
  Pause,
  Stop,
  AddTime,
  PitSolenoid,
  PitOpen,
  PitClose,
  PitDisable
};

enum Color {
  Red,
  Blue,
  Yellow,
  Green,
  White,
  Black
};

enum PitState {
  Open,
  Opening,
  Closing,
  Closed
};
#define PIT_MOTOR_PWM_FWD 2000
#define PIT_MOTOR_PWM_REV 1000
#define PIT_MOTOR_PWM_STOP 1500

// Application State
MatchState matchState = Ready;
unsigned short matchIntervalId = 0;
int elapsedTime = 0;
int addedTime = 0;
int totalMatchDuration = 0;
int remainingTime = 0;
bool pitEnabled = true;
PitState pitState = Closed;
unsigned short addTimeButtonTimeout = 0;
int motorPWM = 0;

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t timerDisplayValue;
  bool buzzerOn;
};
SEND_DATA_STRUCTURE remoteData;
// Init Libs
CRGB leds[NUM_LEDS];
AsyncTimer t;
Adafruit_7segment clockDisplay = Adafruit_7segment();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo pitMotor;  // create servo object to control a servo
EasyTransfer ET;

// Arduino Setup
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Setup");
  // PINS - Input
  pinMode(PIN_BUTTON_START, INPUT_PULLUP);
  pinMode(PIN_BUTTON_PAUSE, INPUT_PULLUP);
  pinMode(PIN_BUTTON_STOP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_ADD_TIME, INPUT_PULLUP);
  pinMode(PIN_PIT_OPEN, INPUT_PULLUP);
  pinMode(PIN_PIT_CLOSE, INPUT_PULLUP);
  pinMode(PIN_PIT_DISABLE, INPUT_PULLUP);
  pinMode(PIN_PIT_SOLENOID_ENABLE, INPUT_PULLUP);
  // PINS - Output
  pinMode(PIN_PIT_OPEN_LED, OUTPUT);
  pinMode(PIN_PIT_CLOSE_LED, OUTPUT);
  pinMode(PIN_RS485_ENABLE, OUTPUT);
  pinMode(PIN_RGB_LED_STRIP, OUTPUT);
  pinMode(PIN_MOTOR_ESC, OUTPUT);
  pinMode(PIN_RELAY_SOLENOID, OUTPUT);
  digitalWrite(PIN_RS485_ENABLE, HIGH);
  ET.begin(details(remoteData), &Serial1);
  clockDisplay.begin(CLOCK_DISPLAY_1_ADDRESS);
  FastLED.addLeds<WS2812, PIN_RGB_LED_STRIP, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(50);
  pitMotor.attach(PIN_MOTOR_ESC);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  // Display static text
  display.println("Michigan");
  display.println("Mashup");
  display.display();
  clockDisplay.print(1234, DEC);
  clockDisplay.writeDisplay();
  delay(1000);
  display.clearDisplay();
  clockDisplay.clear();
  clockDisplay.writeDisplay();
  display.display();
  setLEDs(White);
  remainingTime = MATCH_DURATION;
  updateTimerDisplay();
  Serial.println("Done Setup");
}

// Main Loop
void loop() {
  t.handle();  //timers
  totalMatchDuration = MATCH_DURATION + addedTime;
  remainingTime = totalMatchDuration - elapsedTime;

  // Input Handlers
  handleStartButton();
  handlePauseButton();
  handleStopButton();
  handleAddTimeButton();
  handlePitManualOpenButton();
  handlePitManualCloseButton();
  handlePitManualSolenoidButton();
  handlePitDisableButton();


  tryStopMotor();

  if (matchState == Ready) {
  }

  // If the match is running and we haven't started the loop, start it.
  if (matchState == Running && matchIntervalId == 0) {
    matchIntervalId = t.setInterval(matchLoop, 1000);
  }

  updateOLED();
  updateTimerDisplay();
  ET.sendData();
  delay(50);
}

// The main loop for the match logic.
void matchLoop() {
  Serial.print("Match state: ");
  Serial.println(matchState);
  if (matchState == Running) {
    elapsedTime += 1;

    Serial.print("Tick: ");
    Serial.println(elapsedTime);
    // updateTimerDisplay();
  }
  if (pitEnabled && elapsedTime == PIT_OPEN_AFTER_TIME) {
    handleOpenPit();
  }
  if (elapsedTime == MATCH_DURATION - STOP_SEQUENCE_DURATION) {
    runPreStopSequence();
  }
  if (remainingTime <= 0) {
    stopMatch();
  }
}
