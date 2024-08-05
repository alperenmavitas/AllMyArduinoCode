#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, A4, A5);
// Arduino çeşidine bağlı olarak SDA SDC pinler farklı olabilir.
 
void setup()
{
lcd.begin(16, 2);
lcd.backlight();
 
// Ekrana yazdırılacak metin
lcd.print("Hello");
delay(2500);
}
 
void loop()
{
  lcd.clear();
  lcd.print("Blyatiful");
  delay (1500);
  lcd.clear();
  lcd.print("World");
  delay (1500);
}
