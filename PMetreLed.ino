#define led 3
#define pot A0

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int deger = analogRead(pot);
  deger = deger/4;
  analogWrite(led, deger);

}
