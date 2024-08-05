#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "FS.h"

IPAddress ConfigurationApiIP(192, 168, 4, 1);

const int SETUP_MODE = 0;
const int TEST_MODE = 1;
const int NORMAL_MODE = 2;

const String DataPath = "/data.txt";

int Mode = SETUP_MODE;

String Ap_Wifi_SSID = "ESP-Configuration";
String Ap_Wifi_PASS = "Oktilyon";

ESP8266WebServer server(ConfigurationApiIP, 80);

//Your Domain name with URL path or IP address with path
String serverName = "http://fire-guard.herokuapp.com";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 600000;

struct DeviceData
{
  String targetWifiSSID = "";
  String targetWifiPassword = "";
  String deviceID = "";
  bool isDeviceRegistered = false;
  int sendMessageCount = 0;
};

DeviceData data;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);

  delay(2500);
  Serial.begin(115200);
  SPIFFS.begin();

  if (!SPIFFS.exists(DataPath))
  {
    Serial.println("Device Started on Setup Mode");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(Ap_Wifi_SSID, Ap_Wifi_PASS);

    server.on("/configure/", handleForm);
    server.on("/", []()
              { server.send(200, "text/plain", "Configuration Set-up in Progress"); });
    server.begin();

    Mode = SETUP_MODE;
  }
  else
  {
    Serial.println("Device Started on Normal Mode");

    data = ReadDeviceData();

    WiFi.mode(WIFI_STA);
    WiFi.begin(data.targetWifiSSID, data.targetWifiPassword);

    Serial.println("Connecting to : " + data.targetWifiSSID + " With : " + data.targetWifiPassword + " ");

    Mode = NORMAL_MODE;
  }
}

void handleForm()
{
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else
  {
    DeviceData _data;

    _data.targetWifiSSID = server.arg("targetWifiSSID");
    _data.targetWifiPassword = server.arg("targetWifiPassword");

    SaveDeviceData(_data);

    server.send(200, "text/plain", "Device Configuration Complating ...");

    delay(1000);

    setup();
  }
}

int light_count = 100;
void loop()
{
  if (digitalRead(0) == LOW)
  {
    SPIFFS.remove(DataPath);
    ESP.restart();
  }
  else if (Mode == SETUP_MODE)
  {
    if (light_count > 10)
    {
      light_count--;
    }
    else if (light_count > 0)
    {
      digitalWrite(LED_BUILTIN, 1);
      light_count = -100;
    }

    if (light_count < -10)
    {
      light_count++;
    }
    else if (light_count < 0)
    {
      digitalWrite(LED_BUILTIN, 0);
      light_count = 100;
    }

    delay(10);
    server.handleClient();
  }
  else if (Mode == NORMAL_MODE)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if ((millis() - lastTime) > timerDelay || !data.isDeviceRegistered || data.sendMessageCount < 1)
      {
        //Check WiFi connection status
        if (WiFi.status() == WL_CONNECTED)
        {
          if (data.isDeviceRegistered)
          {
            Serial.print("Registered Device ID : ");
            Serial.println(data.deviceID);
            SendSensorData();
          }
          else
          {
            Serial.println("Device not registered...");
            RegisterDevice();
          }
        }

        lastTime = millis();
      }
    }
    else
    {
      delay(250);
      Serial.print(".");
    }
  }
}

void RegisterDevice()
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName + "/device/register/");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST("apikey=TTNET_ZyXEL_KW4R&device_type=ESP-01&device_mac=" + WiFi.macAddress());

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();

    if (httpResponseCode == 200)
    {
      data.deviceID = payload;
      data.isDeviceRegistered = true;

      Serial.print("Device Registered as : ");
      Serial.println(payload);
    }
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void SendSensorData()
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName + "/device/sensordata/");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST("apikey=TTNET_ZyXEL_KW4R&device_type=ESP-01&device_id=" + data.deviceID + "&device_mac=" + WiFi.macAddress() + "&sensor_data=Temperature%20:%20" + String(ReadSensorData(), 2));

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();

    if (httpResponseCode == 200)
    {
      data.sendMessageCount++;
      Serial.print("Device Sensor Data Send : Temperature : " + String(ReadSensorData(), 2));
      Serial.println(payload);

      ESP.deepSleep(3600e6);
    }
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

float ReadSensorData()
{
  int analogValue = analogRead(A0);
  float millivolts = (analogValue / 1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts / 10;
  return celsius;
}

void SaveDeviceData(DeviceData _data)
{
  File f = SPIFFS.open(DataPath, "w");

  DynamicJsonDocument doc(512);

  doc["1"] = _data.targetWifiSSID;
  doc["2"] = _data.targetWifiPassword;

  serializeJson(doc, f);
  serializeJson(doc, Serial);
  Serial.println("");

  Serial.print("Saved File Size : ");
  Serial.println(f.size());

  f.close();
}

DeviceData ReadDeviceData()
{
  DeviceData _data;
  File f = SPIFFS.open(DataPath, "r");

  if (!f)
  {
    return _data;
  }

  String dataString = f.readString();

  DynamicJsonDocument doc(512);
  deserializeJson(doc, dataString);

  _data.targetWifiSSID = String(doc["1"]);
  _data.targetWifiPassword = String(doc["2"]);

  Serial.print("Readed Data : ");
  Serial.println(dataString);
  f.close();

  return _data;
}
