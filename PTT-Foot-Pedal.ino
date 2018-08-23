// Credit to Mauricio Allayme - Vargtassen@Hellscream-EU for example code provided at
// https://www.instructables.com/id/Arduino-foot-pedal-PTT-switch/
// Created 20 May 2014
// Updated 29 July 2018 by Will Wermager - will@wermager.me
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
  if (digitalRead(pttPin) == LOW) {
    Keyboard.press(pttKey);
    digitalWrite(ledPin, HIGH);
  }
  
// When pedal switch is released, pttPin is pulled high releasing the keypress.
  else {
    Keyboard.releaseAll();
    digitalWrite(ledPin, LOW);
  }
}
