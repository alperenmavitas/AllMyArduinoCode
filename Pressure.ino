#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define pot A0
//Pil için farklı analog giriş ve hesap belirle.

int buzzerPin = 8;
 
LiquidCrystal_I2C lcd(0x27, A4, A5);
// Arduino çeşidine bağlı olarak SDA SDC pinler farklı olabilir.
//338 max = 1.6v
//226 min = 1.2V
void setup()
{
  Serial.begin(9600);
lcd.begin(16, 2);
lcd.backlight();


}
 
void loop(){
  int deger = analogRead(pot);
  
  lcd.setCursor(0,0);
  lcd.print("Kuvvet Olcer"); //Üstteki yazı
  lcd.setCursor(0,1);
  lcd.print("Kuvvet: %"); //Pil seviyesi önyazısı
  if(deger <125){
    lcd.print("Zayif");
  }
  if(deger>125 && deger<375){
    lcd.print("Orta");
  }
  if(deger>375){
    lcd.print("Guclu");
    tone(buzzerPin, 600);
    delay(500);
    noTone(buzzerPin);
    delay(500);
  }
  Serial.println(deger);
 delay(750);
 lcd.clear(); 
}
