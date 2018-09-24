// Updated 23 September 2018 by Will Wermager - will@wermager.me
// Code is compatible only with ATmega32u4 based Arduinos such as the Leonardo or Micro.

#include <Keyboard.h>
int pttPin = 7;

// Keybinds
char pttKey = KEY_CAPS_LOCK; // Desired push-to-talk key
char quickKey = KEY_DOWN_ARROW; // Desired key for quick press

// Button variables
bool buttonVal = HIGH;  // value read from button
bool buttonLast = HIGH; // buffered value of the button's previous state
// Timing variables
int debounce = 20;      // prevent flickering
int holdTime = 250;     // ms hold period: how long to wait for press+hold event
long downTime = -1;     // time the button was pressed down
long upTime = -1;       // time the button was released

void setup() {
  pinMode(pttPin, INPUT_PULLUP);
  Keyboard.begin();
  delay(5000);
}

void loop() {
  int pressType = keyPress();
  if (pressType == 1) quickPress();
  if (pressType == 2) holdKeyEvent();
  //if (pressType == 3) doubleClick(); //TODO
}

void quickPress() {
  // Event 1
  Keyboard.press(quickKey);
  Keyboard.release(quickKey);
  delay(10); // Small delay to prevent event from being re-triggered immediately
}
bool pttPressed = false;
void holdKeyEvent() {
  // Event 2
  Keyboard.press(pttKey);
  while (digitalRead(pttPin) == LOW) {
    //DO NOTHING
  }
  Keyboard.releaseAll();
  delay(10); // Small delay to prevent event from being re-triggered immediately
}

void doubleClick() {
  // TODO
}

int keyPress() {
  // Event = 1 quick press i.e. accidental - anything less than 250 ms
  // Event = 2 Press and hold - 250 ms to actuate
  // TODO Event = 3 Quick double-press - 2 clicks within 250 ms

  int event = 0;
  buttonVal = digitalRead(pttPin);

  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH  && (millis() - upTime) > debounce) {
    downTime = millis();
  }
  // Button Released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce) {
    upTime = millis();
    if (upTime - downTime < holdTime) {
      event = 1;
    }
  }
  // Check for press+hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
    event = 2;
  }
  buttonLast = buttonVal;
  return event;
}
