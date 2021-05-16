#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String apiKey = "<thingspeak_api_key>";

const char *ssid =  "<wifi_ssid>";
const char *pass =  "<wifi_password>";
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully!");
  delay(50);
}

void loop()
{
  Serial.println("\nRequesting parameters...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  int temp = sensors.getTempCByIndex(0);
  if (client.connect(server,80))
  {
    String postStr = apiKey;
    postStr +="&field2=";
    postStr += String(temp);
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

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println("\nSend to Thingspeak.");
  }
  client.stop();

  Serial.println("Waiting...");
  delay(10000);
}
