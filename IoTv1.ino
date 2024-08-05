#include <SoftwareSerial.h>                                   //SoftwareSerial kütüphanimizi ekliyoruz.                                  
#define pres A0

String agAdi = "TurkTelekom_TP1F9A_2.4GHz";                    //Ağımızın adını buraya yazıyoruz.    
String agSifresi = "mK4Ls7RJh7eK";                            //Ağımızın şifresini buraya yazıyoruz.

int rxPin = 11;                                               //ESP8266 RX pini
int txPin = 10;                                               //ESP8266 TX pini

String ip = "184.106.153.149";                                //Thingspeak ip adresi

SoftwareSerial esp(rxPin, txPin);                             //Seri haberleşme pin ayarlarını yapıyoruz.

void setup() {  
  
  Serial.begin(9600);  //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Started");
  esp.begin(115200);                                          //ESP8266 ile seri haberleşmeyi başlatıyoruz. ***
  esp.println("AT");                                          //AT komutu ile modül kontrolünü yapıyoruz.
  Serial.println("AT Yollandı");
  while(!esp.find("OK")){                                     //Modül hazır olana kadar bekliyoruz. Ünlem, şart sağlanana kadar anlamını taşır.
    esp.println("AT");
    Serial.println("ESP8266 Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                                 //ESP8266 modülünü client olarak ayarlıyoruz.
  while(!esp.find("OK")){                                     //Ayar yapılana kadar bekliyoruz.
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    //Ağımıza bağlanıyoruz.
  while(!esp.find("OK"));                                     //Ağa bağlanana kadar bekliyoruz.
  Serial.println("Aga Baglandi.");
  delay(1000);
}
void loop() {
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //Thingspeak'e bağlanıyoruz.
  if(esp.find("Error")){                                      //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("AT+CIPSTART Error");
  }
  
  int force = analogRead(pres);
  
  String veri = "GET https://api.thingspeak.com/update?api_key=IDFYTPJAUU26KLX6";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.
  veri += "&amp;field1=";
  veri += String(force);
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
  esp.println(veri.length()+1);
  delay(2000);
  if(esp.find("OK")){                                          //ESP8266 hazır olduğunda içindeki komutlar çalışıyor.
    esp.print(veri);                                          //Veriyi gönderiyoruz.
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(1000);
  }
  Serial.println("Baglantı Kapatildi.");
  esp.println("AT+CIPCLOSE");                                //Bağlantıyı kapatıyoruz. Bağlantı kapatılmalı.
  delay(10000);                                              //Yeni veri gönderimi için 1 dakika bekliyoruz.
}
