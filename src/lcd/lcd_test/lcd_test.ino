#include <Wire.h>
#include <LiquidCrystal.h>

// Define LCD dimensions (change these according to your LCD)
#define LCD_COLUMNS 20
#define LCD_ROWS    2 //Double check this value.

// Create an LCD object (8bit mode)
LiquidCrystal lcd(32, 31, 40, 39, 38, 37, 36, 35, 34, 33); // Pins (LCD): RS, EN, D0, D1, D2, D3, D4, D5, D6, D7. 
                                                    // Pins (TIVA): PD7, PF4, PF2, PF3, PB3, PC4, PC5, PC6, PC7, PD6.
                                                    //(Match with schematic).

void setup() {
  // Initialize the LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS); //default charsize: LCD_5x8DOTS

  // Print a welcome message
  lcd.print("LCD Test"); //Check if base is HEX.
  lcd.setCursor(0, 1);
  lcd.print("Hello, World!");
}

void loop() {
  // main code goes here
  // For example, you can update the display based on sensor readings or other events
}
