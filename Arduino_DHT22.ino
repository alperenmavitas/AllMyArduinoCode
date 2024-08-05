#include <Adafruit_Sensor.h>
#include <DHT.h>

#define BUTTON_PIN 6  // ESP32 pin GIOP16, which connected to button
#define LED_PIN    4  // ESP32 pin GIOP14, which connected to led
#define buzzerPin  7  // Buzzer Pin GPIO 27

#define DHT_SENSOR_PIN  2 // ESP32 pin GIOP34 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22 //DHT22 Type must be declared

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// The below are variables, which can be changed
int button_state = 0;   // variable for reading the button status

void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the button pin as an pull-up input:
  // the pull-up input pin will be HIGH when the button is open and LOW when the button is pressed.
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);                                         //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Started");
  dht_sensor.begin(); //initialize the DHT sensor
}

void loop() {
  // read the state of the button value:
  button_state = digitalRead(BUTTON_PIN);

  float hum   = dht_sensor.readHumidity();    //Read humidity
  float tempC = dht_sensor.readTemperature(); //Read temperature in Celcius. can read fahrenheit  with (true) in paranteses (Dont use it!)

  if(isnan(tempC) || isnan(hum)){            //Check the reading is successful or not
    Serial.println("Failed to read DHT sensor!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
  }
  
  if (button_state == LOW){       // if button is pressed
    digitalWrite(LED_PIN, LOW); // turn on LED
                                          //Seri port ile haberleşmemizi başlatıyoruz.
  Serial.println("Input is LOW");
  }
  else{                           // otherwise, button is not pressing
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Input is HIGH");// turn off LED
    tone(buzzerPin, 300);
    delay(20);
    noTone(buzzerPin);
    
  }
delay(5000);
}
