// Developed By M V Subrahmanyam
// Electrinics Innovation
#include <Adafruit_Sensor.h>

#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <dummy.h>
#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
// ***********************************************Thingspeak Credentials*****************
const char* apiKey = "OQMM3O5EN7P85CWZ"; //Write API key of your ThingSpeak channel
const char* server = "184.106.153.149"; // API for thingspeak
unsigned long myChannelNumber = 1714529;
//****************************************************************************************
const char *ssid = "TurkTelekom_TP1F9A_2.4GHz"; // Wifi SSID of your Internet connection
const char *pass = "mK4Ls7RJh7eK"; // Password
#define DHTPIN 3 //pin where the DHT22 is connected
DHT dht(DHTPIN, DHT22);
WiFiClient client;

void setup()
{
Serial.begin(9600); // Serial monitor Baudrate
delay(10);

//*********************************************************
dht.begin();
Serial.println("Trying to Connect with ");
Serial.println(ssid);
WiFi.begin(ssid, pass); // Connecting ESP8266 with Internet enabled Wifi with above mentioned credentials
while (WiFi.status() != WL_CONNECTED)
{
// If the connection was unsuccesfull, it will try again and again
delay(500);
Serial.print(".");
}
// Connection succesfull
Serial.println("");
Serial.println("WiFi connected");
}
void loop()
{
int h = dht.readHumidity(); // Reading Temperature form DHT sensor
int t = dht.readTemperature(); // Reading Humidity form DHT sensor
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}
// Connecting to the Thingspeak API and Posting DATA
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
{
// Format of DATA Packet "Write API Key&amp;field1=Temperature data&amp;field2=Humidity Data"
ThingSpeak.setField(1, t);
ThingSpeak.setField(2, h);
  
  int x = ThingSpeak.writeFields(myChannelNumber, "apiKey");
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }


String postStr = apiKey;
postStr +="&amp;field1=";
postStr += String(t);
postStr +="&amp;field2=";
postStr += String(h);
postStr += "\r\n\r\n";
client.println("POST /update HTTP/1.1\n");
client.println("Host: api.thingspeak.com\n");
client.println("Connection: close\n");
client.println("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.println("Content-Type: application/x-www-form-urlencoded\n");
client.println("Content-Length: ");
client.println(postStr.length()+2);
client.println("\n\n");
client.println(postStr);
client.flush();
Serial.println(postStr);
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius, Humidity: ");
Serial.print(h);
Serial.println("%.");
Serial.println("Data has been sussecfully sent to Thingspeak.");
}
client.stop();
Serial.println("Waiting to initiate next data packet...");
// thingspeak needs minimum 15 sec delay between updates
delay(60000);
}
