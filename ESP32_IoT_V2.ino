#include <WiFi.h>
String apiKey = "OQMM3O5EN7P85CWZ";                  //  Enter your Write API key from ThingSpeak
String answer = "0";
const char *ssid =  "TurkTelekom_TP1F9A_2.4GHz";                                    // replace with your wifi ssid and wpa2 key
const char *pass =  "mK4Ls7RJh7eK";
const char* server = "api.thingspeak.com";
WiFiClient client;
void setup() 
{
    Serial.begin(115200);
    delay(10);
    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
 
}
void loop() 
{
  
int h = 0;
float t =0;
float p = 0;
h = random(100);
t = random(85);
p = random(2000); //changing temperature parameter to celsius
 if (client.connect(server,80))                                 //   "184.106.153.149" or api.thingspeak.com
    {  
                            
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(p);
    postStr += "&field3=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println( postStr );
    Serial.print("Pres: ");
    Serial.println(p);
    Serial.print("Temp:");
    Serial.print(t);
    Serial.println(" C");
    Serial.print("Hum: ");
    Serial.println(h);                         
    Serial.println("%. Send to Thingspeak.");
}

else
{
  Serial.println("Connection Failed");
}
    client.stop();
    Serial.println("Waiting...");
    delay(20000);
}
