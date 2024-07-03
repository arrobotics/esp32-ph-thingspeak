#include <SoftwareSerial.h>
#include <Wire.h>
#include <WiFi.h>

 
#define RE 22
#define DE 23
 
const byte ph[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
byte values[11];
SoftwareSerial mod(12, 13);

String apiKey = "N9H4LUI3FW62M33H";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "AR RoboTics";     // replace with your wifi ssid and wpa2 key
const char *pass =  "#G25Rana";
const char* server = "api.thingspeak.com";

WiFiClient client;
 
void setup()
{
  Serial.begin(9600);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
 
  delay(3000);

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
  byte val;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(ph, sizeof(ph)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 11; i++)
    {
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  float soil_ph = float(values[4]) / 10;
  Serial.print(" Soil Ph: ");
  Serial.println(soil_ph, 1);

  
  delay(3000);

  
    if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(soil_ph);
    postStr += "\r\n";
    delay(500);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(500);
  }
  client.stop();
}
