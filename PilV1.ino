#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define pot A0

int buzzerPin = 8;
int min_deger = 226;
float yuzde = 0;
float deger = 0;
 
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
  int gerilim = analogRead(pot);
  if (gerilim>=338){
    gerilim = 338;
  }
  if (gerilim<=226){
    gerilim=226;
  }
  yuzde = gerilim - min_deger;
  deger = (100/92)*yuzde;
  
  
  lcd.setCursor(0,0);
  lcd.print("Pil Olcer"); //Üstteki yazı
  lcd.setCursor(0,1);
  lcd.print("Doluluk: %"); //Pil seviyesi önyazısı
  lcd.print(deger); //LCD ye data gönderir.
  Serial.println(deger);
 if(deger<15){
    //tone(buzzerPin, 600);
   //delay(500);
   // noTone(buzzerPin);
   // delay(500);
 }
 delay(750);
 lcd.clear(); 
}
