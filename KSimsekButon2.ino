int led[]= {8, 9, 10, 11, 12};
#define Button 5
int i = 0;

void setup() {
  for (int i=0; i<5; i++){
    pinMode(led[i], OUTPUT);
  }
  pinMode(Button, INPUT);
}

void loop() {
  while(digitalRead(Button) == 1) {
    digitalWrite(led[i], HIGH);
    i= i++;
  }
}
