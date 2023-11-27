#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {A2, A3, A4, A5}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A6, A7}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long debounceTime = 50; // Adjust this value as needed

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(A0), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(A1), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(A6), keypadInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(A7), keypadInterrupt, FALLING);
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
