//Temperature libraries
#include <OneWire.h>
#include <DallasTemperature.h>
//WiFi library
#include <ESP8266WiFi.h>

//DS18B20 data pin
#define ONE_WIRE_BUS 2

//Temperature library objects' creation
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Heart rate sensor realted declarations
int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;

//ThingSpeak API key
String apiKey = "<thingspeak_api_key>";

//Credentials
const char *ssid =  "<wifi_ssid>";
const char *pass =  "<wifi_name>";
const char* server = "api.thingspeak.com";

//ESP8266 WiFi library object
WiFiClient client;

void setup()
{
  //Setup baud rate for communication
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Connecting to WiFi
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
  //Request temperature sensor's value
  Serial.println("\nRequesting parameters from DS18B20...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  int temp = sensors.getTempCByIndex(0);

  //Computing BPM
  Serial.println("\nComputing BPM...");
  reading = analogRead(0);
  if(reading > UpperThreshold && IgnoreReading == false)
  {
    if(FirstPulseDetected == false)
    {
      FirstPulseTime = millis();
      FirstPulseDetected = true;
    }
    else
    {
      SecondPulseTime = millis();
      PulseInterval = SecondPulseTime - FirstPulseTime;
      FirstPulseTime = SecondPulseTime;
    }
    IgnoreReading = true;
  }
  if(reading < LowerThreshold)
  {
    IgnoreReading = false;
  }
  BPM = (1.0/PulseInterval) * 60.0 * 1000;
  /*Serial.print(reading);
    Serial.print("\t");
    Serial.print(PulseInterval);
    Serial.print("\t");*/
  Serial.print(BPM);
  Serial.println(" BPM");

  //Send data to thingspeak server
  if (client.connect(server,80))
  {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(BPM);
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

  Serial.println("Waiting for 15 second to send next pair of values...");
  delay(15000);
}
