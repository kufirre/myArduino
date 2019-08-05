#include "EmonLib.h"             // Include Emon Library
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
//#include "SoftwareSerial.h"
//#include <NeoSWSerial.h>
#include <AltSoftSerial.h>
//#include <SimpleKalmanFilter.h>

#define VOLT_CAL 462.3  // formerly 466.7
#define CURRENT_CAL 15.152

IPAddress server(192, 168, 43, 204);
char ssid[] = "otonye";           // your network SSID (name)
char pass[] = "12345678";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

int relay = 5;
unsigned long lastSend;


EnergyMonitor emon1;             // Create an instance
WiFiEspClient espClient;        // Initialize the Ethernet client object
PubSubClient client(espClient);
//SoftwareSerial soft(8, 9); // RX, TX
AltSoftSerial soft;
//NeoSWSerial soft(8, 9);

//SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

void setup()
{
  Serial.begin(57600);
  soft.begin(57600);    // initialize serial for ESP module

  emon1.voltage(A0, VOLT_CAL, 1.4);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A1, CURRENT_CAL);       // Current: input pin, calibration.

  pinMode(relay, OUTPUT);

  // initialize ESP module
  WiFi.init(&soft);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  //connect to MQTT server
  client.setServer(server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
    client.loop();
    client.setCallback(callback);
    delay(80);
  }
  client.loop();
  delay(80);
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    //    client.loop();
    //    delayMicroseconds(10000);
    getAndSendPowerData();
    client.loop();
    delay(80);
    lastSend = millis();
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String data;
  for (int i = 0; i < length; i++) {
    //    Serial.print((char)payload[i]);
    data += (char)payload[i];
    if (data == "OFF")
      digitalWrite(relay, HIGH);
    if (data == "ON")
      digitalWrite(relay, LOW);
  }
  //  Serial.print(data);
  Serial.println();
  data = " ";

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect, just a name to identify the client
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world from Arduino");   ///// Change this to the sensor values in json format
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void getAndSendPowerData()
{
  emon1.calcVI(20, 2000);                         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.calcF(50, 2000);     // 50 zero crossings
  double currentDraw     = emon1.Irms;             //extract Irms into Variable
//  currentDraw = simpleKalmanFilter.updateEstimate(currentDraw);
  double supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
//  supplyVoltage = simpleKalmanFilter.updateEstimate(supplyVoltage);
  //  float currentDraw     = 4.00;             //extract Irms into Variable
  //  int supplyVoltage   = 220;             //extract Vrms into Variable
  //  float freq = 50.00;
  double freq = emon1.frequency;
  double RP = emon1.realPower;
  double AP = emon1.apparentPower;
  double PF = emon1.powerFactor;

  int state = digitalRead(relay);

  String voltage = String(supplyVoltage);
  String current = String(currentDraw);
  String frequency = String(freq);
  String apparentPower = String(AP);
  String realPower = String(RP);
  String powerFactor = String(PF);

  String pinState = String(state);

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"voltage\":"; payload += voltage; payload += ",";
  payload += "\"current\":"; payload += current; payload += ",";
  payload += "\"frequency\":"; payload += frequency; payload += ",";
  payload += "\"pinState\":"; payload += pinState; payload += ",";
//  payload += "\"realPower\":"; payload += realPower; payload += ",";
  payload += "\"apparentPower\":"; payload += apparentPower; payload += ",";
  payload += "\"pinState\":"; payload += pinState; //payload += ",";
//  payload += "\"powerFactor\":"; payload += powerFactor;
  payload += "}";

  // Send payload
  char attributes[150];
  payload.toCharArray( attributes, 150);
  client.publish("outTopic", attributes);
//  delay(100);
  Serial.println(attributes);
}
