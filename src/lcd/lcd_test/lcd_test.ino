#include <Wire.h>
#include <LiquidCrystal.h>

// Define LCD dimensions (change these according to your LCD)
#define LCD_COLUMNS 20
#define LCD_ROWS    1 //Double check this value.

// Create an LCD object (8bit mode)
LiquidCrystal lcd(PE_0, PC_5, PB_4, PB_5, PB_6, PB_7); // Pins (LCD): RS, EN, D0, D1, D2, D3, D4, D5, D6, D7. 

void setup() 
{
  // Initialize the LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS); //default charsize: LCD_5x8DOTS
}

void loop() 
{
  // main code goes here
  lcd.print("LCD STRING 1"); //Check if base is HEX.
  lcd.setCursor(0, 1);
  delay(1000);

  lcd.clear();

  lcd.print("LCD STRING 2");
  lcd.setCursor(0, 1);
  delay(1000);

  lcd.clear();
  // For example, you can update the display based on sensor readings or other events
}
