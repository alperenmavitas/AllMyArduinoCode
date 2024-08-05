#define led 3
#define pot A0

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int deger = analogRead(pot); //pot dan alınan değeri, değer degiskenine atar
  deger = map(deger,0,1023,0,255); //deger degiskeni ve aralığını, son iki belirtilen sayı aralığına alır
  analogWrite(led, deger); //led'e deger'deki analog değeri yazar.

}
