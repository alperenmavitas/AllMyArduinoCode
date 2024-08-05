
#define Button 13
#define Led 12

void setup() {
  pinMode(Button, INPUT);
  pinMode(Led, OUTPUT);
  Serial.begin(9600);                                         //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Started");
}

void loop() {

  if(digitalRead(Button) == 1){
  digitalWrite(Led,HIGH);
  Serial.println("Led is HIGH");
  }
  else
  digitalWrite(Led,LOW);
  Serial.println("Led is LOW");
  delay (1500);
}
