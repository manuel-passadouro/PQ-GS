#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};

byte rowPins[ROWS] = {PB_0, PB_1, PB_2, PB_3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {PE_2, PE_3, PE_4, PE_5}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

unsigned long debounceTime = 50; //(ms) Adjust this value as needed.

void setup() {
  Serial.begin(9600); //Double check baud rate.
  attachInterrupt(digitalPinToInterrupt(PB_0), keypadInterrupt, RISING); //Y1
  attachInterrupt(digitalPinToInterrupt(PB_1), keypadInterrupt, RISING); //Y2
  attachInterrupt(digitalPinToInterrupt(PB_2), keypadInterrupt, RISING); //Y3
  attachInterrupt(digitalPinToInterrupt(PB_3), keypadInterrupt, RISING); //Y4
}

void loop() {
  // Main loop is empty because we are using interrupts
  //char key = keypad.getKey();
}

void keypadInterrupt() {
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();

  char key = keypad.getKey();
    
  if (key != NO_KEY) 
  {
    Serial.println(key);
  }

  if (currentTime - lastDebounceTime > debounceTime) 
  {
    char key = keypad.getKey();
    
    if (key != NO_KEY) 
    {
      Serial.println(key);
    }
    lastDebounceTime = currentTime;
  }
}
