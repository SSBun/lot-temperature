#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"

WiFiClientSecure client;

#define SSID "No,thankyou-2"
#define PASSWORD "beijing_newhome_11.3"

const String LC_ID = "7zw6tQBddCfPvbaVtBGxUy0G-gzGzoHsz";
const String LC_KEY = "voPGHhBz7MozpUm0Fq2AMFmv";
const String URL = "/1.1/classes/TH";
const char fingerprint[] PROGMEM = "D5 79 2E 15 A9 60 9F F1 15 4B 55 96 65 E6 3F 72 44 59 BB 94";

#define SERVER "7zw6tqbd.lc-cn-n1-shared.com"
// #define SERVER "api.github.com"

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
  client.setFingerprint(fingerprint);
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
  // delay(2000);
  // digitalWrite(D1, HIGH);
  // delay(2000);
  // digitalWrite(D1, LOW);
  // ESP.deepSleep(10e6);
  // delay(5000);
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
  if (client.connect(SERVER, 443))
  {
    // String data = "{\"weather\":16,\"humidity\":72}";
    String data = "weather=";
    data += String(temperature);
    data += "&humidity=";
    data += String(humidity);
    // data += "&heatIndex=";
    // data += String(heatIndex);
    String postRequest = (String)("POST ") + URL + " HTTP/1.1\r\n" +
                         "Content-Type: application/json;charset=utf-8\r\n" +
                         "Host: " + SERVER + "\r\n" +
                         "User-Agent: BuildFailureDetectorESP8266\r\n" +
                         "X-LC-Id:" + LC_ID + "\r\n" +
                         "X-LC-Key" + LC_KEY + "\r\n" +
                         "Connection: close\r\n\r\n" +
                         data + "\r\n";
    Serial.println(postRequest);
    client.print(postRequest);
    // client.print("POST /1.1/classes/TH HTTP/1.1\n");
    // client.print("Host: 7zw6tqbd.lc-cn-n1-shared.com\n");
    // client.print("Connection: keep-alive\n");
    // client.print("X-LC-Id: " + LC_ID + "\n");
    // client.print("X-LC-Key: " + LC_KEY + "\n");
    // client.print("Content-Type: application/json\n");
    // client.print("Content-Length: ");
    // client.print(Data.length());
    // client.print("\n\n");
    // client.print(Data);
    // Serial.println("");
    // Serial.println("uploaded to Thingspeak server....");

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
