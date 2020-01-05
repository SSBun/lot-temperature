#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

WiFiClient client;

#define SSID "No,thankyou-2"
#define PASSWORD "beijing_newhome_11.3"

const String URL = "/";

#define SERVER "39.100.226.244"

const int SENSOR_PIN = D1;
#define DHT_TYPE DHT11

DHT dht(SENSOR_PIN, DHT_TYPE);

void echo(String);
void echoln(String);
void uploadData(float, float, float);
void getSensorData(float &, float &, float &);

void setup()
{
  // pinMode(A0, INPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  // echo("setup");
  // ESP.wdtEnable(5000);

  float temperature = -99;
  float humidity = -99;
  float heatIndex = -99;

  dht.begin();
  delay(5000);
  getSensorData(temperature, humidity, heatIndex);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  
  echo("Connecting to ");
  delay(10);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    echo(".");
  }
  echoln("");
  echo("NodeMCU connectd to wifi...");
  uploadData(temperature, humidity, heatIndex);
  Serial.println("deep sleep for 15 seconds");
  // ESP.deepSleep(15e6);
}

void loop()
{
  delay(5000);
  Serial.println("Send request ...");
  uploadData(22, 22, 22);
}

void getSensorData(float &temperature, float &humidity, float &heatIndex)
{
  // 获取湿度值
  humidity = dht.readHumidity();
  // 获取温度值
  temperature = dht.readTemperature();

  heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % ");
  // 温度值
  Serial.print(" Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");

  Serial.print("HeatIndex: ");
  Serial.print(heatIndex);
  Serial.println(" *C");
}

void uploadData(float temperature, float humidity, float heatIndex)
{
  if (client.connect(SERVER, 8080))
  {
    String data = "{\"weather\":16,\"humidity\":72}";
    // String data = "weather=";
    // data += String(temperature);
    // data += "&humidity=";
    // data += String(humidity);

    String postRequest = "GET " + URL + " HTTP/1.1\r\n" +
                         "Content-Type: application/json;charset=utf-8\r\n" +
                         "Host: " + SERVER + "\r\n" +
                         "Connection: close\r\n\r\n" +
                         data + "\r\n";
    Serial.println(postRequest);
    client.print(postRequest);

    String response = client.readString();
    echo("The response is: ");
    echo(response);
    client.stop();
  } else {
    Serial.println("Connect to server failure !!!");
  }
}

void echo(String message)
{
  Serial.print(message);
}

void echoln(String message)
{
  Serial.println(message);
}
