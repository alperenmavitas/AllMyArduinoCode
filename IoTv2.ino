#define ag_ismi "TurkTelekom_TP1F9A_5GHz"
#define ag_sifresi "mK4Ls7RJh7eK"
#define IP "184.106.153.149" //thingspeak IP adresi

int forcepin = A0;

void setup() {
Serial.begin(115200);
Serial.println("AT"); //ESP ile bağlantı olup olmadığının kontrolü
delay(3000);
if(Serial.find("OK")){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String baglantiKomutu = String("AT+CWJAP=\"") + ag_ismi + "\",\"" + ag_sifresi + "\"";
  Serial.println(baglantiKomutu);

  delay(5000);
}
}

void loop() {
  int analog_data = analogRead(forcepin);
  analog_data = (analog_data / 1023) * 5000;
  float force = analog_data / 100.0;
  if(force>5){
    force_send();
  }
//delay ile bekleme süresi belirlenir
delay(10000);
}
void force_send(){
  String veri = "GET https://api.thingspeak.com/update?api_key=IDFYTPJAUU26KLX6";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.
  veri += "&amp;field1=";
  veri += String(force);
  esp.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
  esp.println(veri.length()+1);
  delay(2000);
  if(esp.find("&gt;")){                                          //ESP8266 hazır olduğunda içindeki komutlar çalışıyor.
    esp.print(veri);                                          //Veriyi gönderiyoruz.
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(1000);
}
