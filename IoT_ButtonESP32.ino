#include <dummy.h>
#include <Blynk.h>  
                   
#define Button 2

String agAdi = "TurkTelekom_TP1F9A_2.4GHz";                    //Ağımızın adını buraya yazıyoruz.    
String agSifresi = "mK4Ls7RJh7eK";                            //Ağımızın şifresini buraya yazıyoruz.

int rxPin = 1;                                               //ESP8266 RX pini
int txPin = 0;                                               //ESP8266 TX pini
int Output;

String ip = "184.106.153.149";                                //Thingspeak ip adresi

void setup() {  

  pinMode(Button, INPUT);
  
  Serial.begin(9600);                                         //Seri port ile haberleşmemizi başlatıyoruz.
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
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //Thingspeak'e bağlanıyoruz.
  if(esp.find("Error")){                                      //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("AT+CIPSTART Error");
  }
  
  
}
void loop() {

  int Toggle = digitalRead(Button);
  if(Toggle == 1){
    Output = 10;
    }
  if(Toggle == 0){
    Output = 5;
    }
    
  
  String veri = "GET https://api.thingspeak.com/update?api_key=WGPKAPTIKWXPVLTO";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.
  veri += "&amp;field1=";
  veri += String(Output);
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
