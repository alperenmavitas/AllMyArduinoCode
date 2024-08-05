#define led1 9
#define led2 11
#define pot A0

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int red = analogRead(pot);
  red = map(red,0,1023,0,255);
  analogWrite(led1, red);
  int green = 255 - red;
  analogWrite(led2, green);
  

}
