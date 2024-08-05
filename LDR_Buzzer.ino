int buzzerPin = 10;
int AnalogInput = A0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int deger = analogRead(AnalogInput);
  int pitch = 200 + deger/4;
  tone(buzzerPin, pitch);

}
