const int buzzerPin = PB_2; // Change this to the actual pin you are using for the buzzer

void setup() {
  pinMode(buzzerPin, OUTPUT);
  //analogWriteResolution(12); // Not in library, defaults to 12 bit.
}

void loop() {
  // Play a rising tone
  for (int i = 0; i <= 255; i++) {
    analogWrite(buzzerPin, i);
    delay(10);
  }

  delay(500); // Pause between rising and falling tones

  // Play a falling tone
  for (int i = 255; i >= 0; i--) {
    analogWrite(buzzerPin, i);
    delay(10);
  }

  delay(500); // Pause between falling and rising tones
}
