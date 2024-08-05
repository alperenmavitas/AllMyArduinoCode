int led[]= {8, 9, 10, 11, 12};
#define Button 5
#define potpin A0
int analogvalue;

void setup() {
  Serial.begin(9600);
  Serial.println("Pot deger okuma");
  for (int i=0; i<5; i++){
    pinMode(led[i], OUTPUT);
  }
  pinMode(Button, INPUT);
}

void loop() {
  analogvalue = analogRead(potpin);
  analogvalue = map(analogvalue, 0, 1023, 0, 5);
  Serial.println(analogvalue);
  delay(1500);
  while(digitalRead(Button) == 1 and analogvalue == 0) {
    for (int i = 0; i < 5; i++) {
    digitalWrite(led[i], HIGH);
    delay(500);
    digitalWrite(led[i], LOW);
  }
   for (int i=4; i>-1; i--){
    digitalWrite(led[i], HIGH);
    delay(500);
    digitalWrite(led[i], LOW);
  }
  }
  for (int i=4; i>-1; i--){
  digitalWrite(led[i], HIGH);
  delay(100);
  digitalWrite(led[i], LOW);
  }
  
  
}
