#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

IPAddress ConfigurationApiIP(192, 168, 4, 1);

const int SETUP_MODE = 0;
const int TEST_MODE = 1;
const int NORMAL_MODE =2;

int Mode = SETUP_MODE;

String Wifi_SSID = "ESP-Configuration";
String Wifi_PASS = "Oktilyon";
String Device_ID = "Not_Configured";

ESP8266WebServer server(ConfigurationApiIP, 80);

void setup(void) {
  EEPROM.begin(512);
  pinMode(1, OUTPUT);
  digitalWrite(1, 1);
  //IF EEPROM[0] = 1 Get Stored Data 
  //ELSE Create Acces Point to user configuration 
  if(char(EEPROM.read(0)) == 1)
  {
    int data_number = 0, addr = 0;
    String data_string = "";
    while(data_number < 3 && addr < 255){
      addr++;
      char data_char = char(EEPROM.read(addr));
      if(data_char == '|')
      {
        if(data_number == 0){
          Wifi_SSID = data_string;
        }else if(data_number == 1){
          Wifi_PASS = data_string;
        }else if(data_number == 2){
          Device_ID = data_string;
        }    
        data_number ++;
        data_string = "";
      }else {
        data_string += data_char;
      }
    }
          
    WiFi.mode(WIFI_STA);
    WiFi.begin(Wifi_SSID, Wifi_PASS);

    server.on("/", []() {
      server.send(200, "text/plain", "Configured Succesfully");
    });

    Mode = NORMAL_MODE;
  }else {     
    WiFi.mode(WIFI_AP);
    WiFi.softAP(Wifi_SSID, Wifi_PASS);
    
    server.on("/configure/", handleForm);
    server.on("/", []() {
      server.send(200, "text/plain", "Configuration Set-up in Progress");
    });

    Mode = SETUP_MODE;
  }

  server.begin();
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String message = "|";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += server.arg(i) + "|";
    }
   
    //Set EEPROM[0] to 1 to control if ESP configured
    EEPROM.write(0, 1);
    for(int i=1;i<message.length();i++)
    {
      EEPROM.write(i, message[i]); //Write one by one with starting address of 1
    }
    EEPROM.commit(); 
    
    server.send(200, "text/plain", message);

    delay(1000);
    
    setup();
  }
}

int light_count = 100;
void loop(void) {
  if(Mode == SETUP_MODE)
  {
    if(light_count > 10)
    {
      light_count--;  
    }else if(light_count > 0)
    {
      digitalWrite(1, 1);   
      light_count = -100;  
    }

    if(light_count < -10)
    {
      light_count++;  
    }else if(light_count < 0)
    {
      digitalWrite(1, 0);
      light_count = 100;  
    }
  }else if(Mode == NORMAL_MODE){
    while(WiFi.status()!= WL_CONNECTED){
      digitalWrite(1, 1);
      delay(50);
      digitalWrite(1, 0);
      delay(50);
    }
    
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://92.42.38.62:9696/");      //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header

    String postData = "ID=" + Device_ID;
    http.POST(postData);   //Send the request
    http.end();  //Close connection

    digitalWrite(1, 1);

    //1,000,000 = 1 second 
    //30 Second Sleep
    //ESP.deepSleep(100000 * 30);
    delay(15000);
  }

  delay(5);
  server.handleClient();
}
