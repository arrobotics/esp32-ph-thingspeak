                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            //#include <SoftwareSerial.h>
#include <Wire.h>
#include <WiFi.h>

// Define the Serial port to communicate with ESP32
#define SerialPort Serial2  // Change this if you're using a different serial port

// Define the queries to send to the ph sensor

uint8_t phQuery[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};

///// thingspeak data

String apiKey = "N9H4LUI3FW62M33H";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "AR RoboTics";     // replace with your wifi ssid and wpa2 key
const char *pass =  "#G25Rana";
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); // Change the pins according to your ESP32's hardware serial pins

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

void loop() {
 
  // Read pH value
  printPHValue();
  delay(1000); // Adjust delay according to your needs

}
void printPHValue() {
  // Send query to the ph sensor
  SerialPort.write(phQuery, sizeof(phQuery));

  // Wait for response from the sensor
  delay(1000); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 7) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t phResponse[7]; // Adjust the size based on the expected response length
    SerialPort.readBytes(phResponse, sizeof(phResponse));

    // Extract pH value from the received response
    uint16_t phValue = (phResponse[3] << 8) | phResponse[4];
    float pH = phValue / 100.0; // Convert to decimal

    // Print the value
    Serial.println("pH Value:");
    Serial.println(pH);
    delay(3000);

     if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(pH);
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
 
}
