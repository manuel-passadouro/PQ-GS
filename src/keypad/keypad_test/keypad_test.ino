#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};

byte rowPins[ROWS] = {PE_5, PB_4, PA_5, PA_6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {PB_5, PB_0, PB_1, PE_4}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long debounceTime = 50; //(ms) Adjust this value as needed.

void setup() {
  Serial.begin(9600); //Double check baud rate.
  attachInterrupt(digitalPinToInterrupt(PB_5), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(PB_0), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(PB_1), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(PE_4), keypadInterrupt, FALLING);
}

void loop() {
  // Main loop is empty because we are using interrupts
}

void keypadInterrupt() {
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTime > debounceTime) {
    char key = keypad.getKey();
    if (key != NO_KEY) {
      Serial.println(key);
    }
    lastDebounceTime = currentTime;
  }
}
