
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the LM4F120XL pin number it is connected to
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
//        PA_5    PA_6    PA_7    PA_2      PA_3     PA_4
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int switchPin = 37; //PC_4
int switchState = 0;
int prevSwitchState = 0;
int reply;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 1);
  pinMode(switchPin,INPUT);
  lcd.print("Ask the ");
  lcd.print("Crystal Ball!");
}

void loop() { 
  switchState = digitalRead(switchPin);
  if (switchState != prevSwitchState) {
    if (switchState == LOW) {
      reply = random(8);
      lcd.clear();
      
    lcd.setCursor(0,0);
    lcd.print("The ball says:");
    delay(1000);
    lcd.autoscroll();
    lcd.setCursor(20,0);
  
    switch(reply){
      case 0:
        lcd.print("Yes           ");
        break;
      case 1:
        lcd.print("Likely        ");
        break;
      case 2:
        lcd.print("Certainly     ");
        break;
      case 3:
        lcd.print("Seems Good    ");
        break;
      case 4:
        lcd.print("Unsure        ");
        break;
      case 5:
        lcd.print("Ask again     ");
        break;
      case 6:
        lcd.print("Doubtful      ");
        break;
      case 7:
        lcd.print("No            ");
        break;
    }
    lcd.noAutoscroll();
    delay(1000);
    lcd.clear();
    lcd.print("Ask the Crystal Ball!");
  }
 }
 prevSwitchState = switchState;
}
