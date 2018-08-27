// Credit to Mauricio Allayme - Vargtassen@Hellscream-EU for example code provided at
// https://www.instructables.com/id/Arduino-foot-pedal-PTT-switch/
// Created 20 May 2014
// Updated 27 August 2018 by Will Wermager - will@wermager.me
// Code is compatible only with ATmega32u4 based Arduinos such as the Leonardo or Micro.
  
#include <Keyboard.h>
// Declaring digital pins for switch input and status LED. Pedal switch is connected between Arduino GND pin and pttPin.
  int pttPin = 7;
  int ledPin = 13;

// Declaring what key to trigger PTT on VoIP application
// Modifier keys can be found at: https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
  char pttKey = KEY_CAPS_LOCK;

// Setting variables.  
void setup() {
  pinMode(pttPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Keyboard.begin();

// Delay to reprogram the microcontroller in case of SNAFU.
  delay(5000);
}

// Loop checks if pttPin is pulled low, then holds the predefined key.
void loop() {
  int pressType = keyPress();
  if (pressType == 1) quickPress();
  if (pressType == 2) holdKeyEvent(); 
//if (pressType == 3) doubleClick(); //TODO
}
void quickPress(){
  // Event 1
  // Could be bound to something else useful later
  if (digitalRead(ledPin) == LOW){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}
void holdKeyEvent() {
  // Event 2
  Keyboard.press(pttKey);
  digitalWrite(ledPin, HIGH);
  while(digitalRead(pttPin) == LOW){
    //DO NOTHING
  }
  Keyboard.releaseAll();
  digitalWrite(ledPin, LOW);
}

void doubleClick(){
  //TODO
}

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int holdTime = 250;        // ms hold period: how long to wait for press+hold event
// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released

int keyPress(){
  // Event = 1 quick press i.e. accidental - anything less than 250 ms
  // Event = 2 Press and hold - 250ms to actuate
  // Event = 3 Quick double-press - 2 clicks within 250 ms
  int event = 0;
   buttonVal = digitalRead(pttPin);
   // Button pressed down

   if (buttonVal == LOW && buttonLast == HIGH  && (millis() - upTime) > debounce){
    downTime = millis();
   }
   // Button Released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce){
    upTime = millis();
    if (upTime - downTime < holdTime){
      event = 1;
    }
   }
   if (buttonVal == LOW && (millis() - downTime) >= holdTime){
    event = 2;
   }
   buttonLast = buttonVal;
   return event;
}

