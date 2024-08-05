int buzzerPin = 7;
int notaSayisi = 8;
int C = 262; //Notaların frekans karşılıkları girilir
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_ = 523;
int notalar[] = {C, D, E, F, G, A, B, C_};

void setup() {
 for (int i = 0; i < notaSayisi; i++)
  {
    tone(buzzerPin, notalar[i]);
    delay(500);
    noTone(buzzerPin);
    delay(20);
  }
  noTone(buzzerPin);

}

void loop() {
  // put your main code here, to run repeatedly:

}
