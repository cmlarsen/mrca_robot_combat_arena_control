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
  Pit Enable/Disable Switch - disables the pit automatically opening.
  Open Pit - releases solenoid via relay and enables pit motor via ESC
  Start  -starts match
  Pause - pause match, press start to resume
  Stop/Reset - stops match;
  Add Time - adds 5 seconds per press;

  * These aren't connected to arduino
  Close Pit - Nothing, wired to the motors.
  Master Power - Disables power to system

  OUTPUTS:
  OLED
  Clocks
  Pit Solenoid Relay
  Buzzer


  BULKHEAD_CONNECTOR_PINOUT:
  1: Red, 12v Gearmotor A 
  2: Black, 12v Gearmotor B
  3: Red, Wasp ESC Limit Switch
  4: Black, Common GND &  Wasp ESC Limit Switch
  5: Red, +12v Solenoid #1
  6: Black, GND Solenoid #1
  7: Yellow, +12v Solenoid #2
  8: White, GND Soleonoid #2
  9: Red, +12v RGB LEDs
  10: Yellow,  5v Data, RGB LEDs

  
**/


/**
Pin 
2 = RS485 Enable
3 = RGBLED data
4 = Disable pit
A3 = Closing LED
A2 = Opening LED

CAT5
Blue - B
Blue Stripe - A
Green: Gnd
Green Stripe: 5v

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
#define PIN_PIT_OPEN_LED 16
#define PIN_PIT_CLOSE_LED 17
#define PIN_RS485_ENABLE 2
#define PIN_RGB_LED_STRIP 3
#define PIN_MOTOR_ESC 15
#define PIN_RELAY_SOLENOID 14
#define PIN_PIT_ENABLE_LED 12

// PINS - I2C
#define PIN_I2C_SDA 18
#define PIN_I2C_SCL 19

// System Config
#define MATCH_DURATION 15         //seconds
#define PIT_OPEN_AT_REMAINING_TIME 5     //seconds into the match that the pit will open
#define STOP_SEQUENCE_DURATION 3  //seconds
#define ADDED_TIME_DURATION 5     //seconds
#define COUNTDOWN_DURATION 3
#define NUM_LEDS 200                    //How many LEDs are there in the strip?
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
  Starting,
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
#define PIT_MOTOR_PWM_FWD 1000
#define PIT_MOTOR_PWM_REV 2000
#define PIT_MOTOR_PWM_STOP 1440

// Application State
MatchState matchState = Ready;
unsigned short matchIntervalId = 0;
int countdownTime = 0;
int elapsedTime = 0;
int addedTime = 0;
int totalMatchDuration = MATCH_DURATION;
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
  pinMode(PIN_PIT_ENABLE_LED, OUTPUT);
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
  delay(500);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  // Display static text
  display.println("Michigan");
  display.println("Mashup");
  display.display();
  updateTimerDisplay(8888);
  delay(1000);
  display.clearDisplay();
  clockDisplay.clear();
  clockDisplay.writeDisplay();
  display.display();
  setLEDs(White);
  remainingTime = MATCH_DURATION;
  updateTimerDisplay(remainingTime);
  Serial.println("Done Setup");
}

// Main Loop
void loop() {
  t.handle();  //timers

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




  // If the match is running and we haven't started the loop, start it.

  if ((matchState == Starting || matchState == Running) && matchIntervalId == 0) {
    matchIntervalId = t.setInterval(matchLoop, 1000);
  }

  // if (matchState == Ready || matchState == Paused){
  if (addedTime >0){
    remainingTime +=addedTime;
    totalMatchDuration +=addedTime;
    addedTime = 0;
  }
  
  
  updateOLED();
  Serial.print(elapsedTime);
  Serial.print(" ");
  Serial.print(totalMatchDuration);
  Serial.print(" ");
  Serial.println(remainingTime);
  updateTimerDisplay(remainingTime);
  ET.sendData();
  delay(28);
}

// The main loop for the match logic.
void matchLoop() {
  if (matchState == Starting) {
    countdownTime += 1;
  }
  if (matchState == Running) {
    elapsedTime += 1;
  }

  
  if (matchState == Running) {
    remainingTime = totalMatchDuration - elapsedTime;
  }
  if (matchState == Starting) {
    remainingTime = COUNTDOWN_DURATION - countdownTime;
  }


  if (matchState == Starting && countdownTime == COUNTDOWN_DURATION) {
    countdownTime = 0;
    elapsedTime = 0;
    remainingTime = totalMatchDuration;
    startMatch();
  }


  if (remainingTime == 0) {
    stopMatch();
  }

  if (pitEnabled && remainingTime == PIT_OPEN_AT_REMAINING_TIME){
    handleOpenPit();
  }
  if(pitEnabled){
    digitalWrite(PIN_PIT_ENABLE_LED, HIGH);
  } else {
    digitalWrite(PIN_PIT_ENABLE_LED, LOW);
    
  }
  if (elapsedTime == totalMatchDuration - STOP_SEQUENCE_DURATION) {
    runPreStopSequence();
  }
}
