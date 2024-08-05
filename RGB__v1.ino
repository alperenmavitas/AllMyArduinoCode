#define red 10
#define green 9
#define blue 8

#define button 5
#define pot A0

int pot_value;


void deger_oku() {
  pot_value = analogRead(pot);

  pot_value = map(pot_value, 0, 1023, 0, 255);
}

void deger_yaz(){
  if (button == 1){
    analogWrite(blue, pot_value);
  }
 
  
}
void setup() {
  pinMode(button, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  deger_oku();
  deger_yaz();

}
