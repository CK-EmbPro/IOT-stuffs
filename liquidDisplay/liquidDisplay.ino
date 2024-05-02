#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
lcd.init();
lcd.clear();
lcd.backlight();
// set the LCD address to 0x27 for a 16 chars and 2 line display
// Make sure backlight is on
// Print a message on both lines of the LCD.
lcd.setCursor(2,0);
//Set cursor to character 2 on line 0
lcd.print("Ck the Chairman");
lcd.setCursor(2,1);
//Move cursor to character 2 on line 1
lcd.print("Hey you got me");
}
void loop() {
}
