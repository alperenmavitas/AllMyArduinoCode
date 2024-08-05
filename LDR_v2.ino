int ledler[] = {
  8, 
  9, 
  10, 
  11, 
  12
};
int led_sayi = 5;
int ldr_pin = A2;
int deger = 0;
int i = 0;
void setup() {
    for (int i = 0; i < led_sayi; i++){
      pinMode(ledler[i], OUTPUT);
    }
  }
void loop() {
  deger = analogRead(ldr_pin);
  if (deger > 0 && deger <= 255){
    digitalWrite(ledler[0], HIGH);
    digitalWrite(ledler[1], LOW);
    digitalWrite(ledler[2], LOW);
    digitalWrite(ledler[3], LOW);
    digitalWrite(ledler[4], LOW);
  }
  if (deger>256 && deger <= 350){
    digitalWrite(ledler[0], HIGH);
    digitalWrite(ledler[1], HIGH);
    digitalWrite(ledler[2], LOW);
    digitalWrite(ledler[3], LOW);
    digitalWrite(ledler[4], LOW);
  }
  if (deger>351 && deger <= 450){
    digitalWrite(ledler[0], HIGH);
    digitalWrite(ledler[1], HIGH);
    digitalWrite(ledler[2], HIGH);
    digitalWrite(ledler[3], LOW);
    digitalWrite(ledler[4], LOW);
  }
  if (deger>451 && deger <= 550){
    digitalWrite(ledler[0], HIGH);
    digitalWrite(ledler[1], HIGH);
    digitalWrite(ledler[2], HIGH);
    digitalWrite(ledler[3], HIGH);
    digitalWrite(ledler[4], LOW);
  }
  if (deger >551 && deger <=1023){
    digitalWrite(ledler[0], HIGH);
    digitalWrite(ledler[1], HIGH);
    digitalWrite(ledler[2], HIGH);
    digitalWrite(ledler[3], HIGH);
    digitalWrite(ledler[4], HIGH);
  }
  delay (100);
}
