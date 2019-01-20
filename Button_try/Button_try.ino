#define BOARD_BUTTON_PIN 28

/*
 Button
define 
 Turns on and off the built-in LED when the built-in button is
 pressed.

 Ported to Maple from the Arduino example 27 May 2011
 by Marti Bolivar
*/

void setup() {
  // Initialize the built-in LED pin as an output:
  pinMode(32, OUTPUT);
  // Initialize the built-in button (labeled BUT) as an input:
  pinMode(BOARD_BUTTON_PIN, INPUT);
}

#define BUTTON_DEBOUNCE_DELAY 1
uint8 isButtonPressed(uint8 pin=BOARD_BUTTON_PIN,
                      uint32 pressedLevel=BOARD_BUTTON_PRESSED_LEVEL) {
    if (digitalRead(pin) == pressedLevel) {
        delay(BUTTON_DEBOUNCE_DELAY);
        while (digitalRead(pin) == pressedLevel)
            ;
        return true;
    }
    return false;
}

void loop() {
    // Check if the button is pressed.
    if (isButtonPressed()) {
        // If so, turn the LED from on to off, or from off to on:
        digitalWrite(32,!digitalRead(32));// Turn the LED from off to on, or on to off
    }
}
