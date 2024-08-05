#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

#define FORCE_SENSOR_PIN 36     //Pin of force sensor
#define BATTERY_PIN 39          //Pin where battery is measured
#define DHT_SENSOR_PIN  3       // ESP32 pin GIOP3(34) connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22   //DHT22 Type must be declared

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

char ssid[] = "TurkTelekom_TP1F9A_2.4GHz";   // your network SSID (name) 
char pass[] = "mK4Ls7RJh7eK";                // your network password
int keyIndex = 0;                            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1714529;
const char * myWriteAPIKey = "OQMM3O5EN7P85CWZ";

int number1 = 0;
int number2 = 0;

String myStatus = "";

void setup() {
  Serial.begin(9600);  //Initialize serial
  dht_sensor.begin(); // initialize the DHT sensor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  int force = analogRead(FORCE_SENSOR_PIN);
  int battery = analogRead(BATTERY_PIN);
  float hum   = dht_sensor.readHumidity();    //Read humidity
  float tempC = dht_sensor.readTemperature(); //Read temperature in Celcius. can read fahrenheit  with (true) in paranteses (Dont use it!)

  if(isnan(hum) || isnan(tempC)){            //Check the reading is successful or not
    Serial.println("Failed to read DHT sensor!");
  }
  else {                                      //If successfull, print the values to serial
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");

    Serial.print("Force: ");
    Serial.print(force);

    Serial.print("Battery: ");
    Serial.print(battery);
  }
  ThingSpeak.setField(1, tempC); //Sets field number and data. If you want to send it, use ThingSpeak.writeFields()
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, force); 
  ThingSpeak.setField(4, battery);

  // set the status
  ThingSpeak.setStatus(myStatus); //Set the status of a multi-field update. Use status to provide additonal details when writing a channel update.
                                  //Also, status can be used by the ThingTweet App to send a message to Twitter.
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  //Writes all set fields? If want to write individually, ThingSpeak.writeField(Channelnum, field, data, writeapikey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(10000); // Wait some seconds to update the channel again
}
