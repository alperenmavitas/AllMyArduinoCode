#define potpin A0 //A0 pinine potansiyometre tanımlama
int deger =0; //"deger" adlı 0 a tanımlı bir değişken
void setup() {
  Serial.begin(9600); //9600 Baund bir seri haberleşme başlatıyoruz
  Serial.println("Pot Deger Okuma"); //Seri monitörde bir kez gönderilen bir mesaj tanımlıyoruz
}
void loop() {
  deger = analogRead(potpin); //"Değer" değişkeni potansiyometrenin değerini okuyup buna göre değişir
  Serial.println(deger); //Okunan değer seri monitörde mesaj olarak gönderilir
  delay(1500); //İşlem belirtilen milisaniyede bir yapılır.
}
