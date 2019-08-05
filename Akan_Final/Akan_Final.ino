#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <SPI.h>
#include "DHT.h"
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


#define DHTPIN 7
#define DHTTYPE DHT11


// WiFi network info.
//char ssid[] = "Rita";           // your network SSID (name)
//char pass[] = "Gorgeous";           // your network password

char ssid[] = "Honor 6X";           // your network SSID (name)
char pass[] = "2121kufy";           // your network password


int status = WL_IDLE_STATUS;   // the Wifi radio's status


WiFiEspClient client;        // Initialize the Ethernet client object
uint32_t ip;

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

// Dweet parameters
#define thing_name  "Akan_IoT_StreetLight"

// Variables to be sent
int temperature;
int humidity;



double distance1, distance2, distance3, duration1, duration2, duration3;

const int light1 = 3;
const int light2 = 4;
const int light3 = 5;

const int trigger1 = 11;
const int trigger2 = 9;
const int trigger3 = 7;

const int echo1 = 10;
const int echo2 = 8;
const int echo3 = 6;

//read the pinStates
bool pinState1 = LOW;
bool pinState2 = LOW;
bool pinState3 = LOW;

//double distance1;
//double distance2;
//double distance3;

void setup()
{
  Serial.begin(115200);
  dht.begin();

  delay(10);

  pinMode(trigger1, OUTPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(trigger3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);

  // Set ESP8266 baud rate
  delay(10);
  WiFi.init(&Serial);

  // attempt to connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //initialice and connect to wifi lan
  WiFi.begin(ssid, pass);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  if (status == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
    Serial.println(F("======================================================"));
  }

  Serial.println(F("Setup ready"));

}

void loop()
{
  // Measure from DHT
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  temperature = (int)t;
  humidity = (int)h;

  Serial.println(F("Measurements done"));

  const int httpPort = 80;

  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
    if ((tm.Hour >= 18 && tm.Hour < 6 && tm.Minute >= 0 && tm.Minute >= 0)) //This determines the time when the ultrasonic sensors start functioning
    {
      distance1 = readFromSensor1();
      distance2 = readFromSensor2();
      distance3 = readFromSensor3();

      if (distance1 <= 30)
        digitalWrite(light1, HIGH);

      if (distance2 <= 30)
        digitalWrite(light2, HIGH);

      if (distance3 <= 30)
        digitalWrite(light3, HIGH);

      else {
        digitalWrite(light1, LOW);
        digitalWrite(light2, LOW);
        digitalWrite(light3, LOW);
      }

    }
    //    } else {
    //      if (RTC.chipPresent()) {
    //        Serial.println("The DS1307 is stopped.  Please run the SetTime");
    //        Serial.println("example to initialize the time and begin running.");
    //        Serial.println();
    //      } else {
    //        Serial.println("DS1307 read error!  Please check the circuitry.");
    //        Serial.println();
    //      }
    //      delay(3000);
    //    }
  }////////////////////////////////////// end of time function

  bool  pinState1 = digitalRead(light1);
  bool  pinState2 = digitalRead(light2);
  bool  pinState3 = digitalRead(light3);
  client.flush();
  client.stop();
  if (!client.connect("dweet.io", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print(F("Sending request... "));
  while (client.connected()) {
    client.print(F("GET /dweet/for/"));
    client.print(thing_name);
    client.print(F("?temperature="));
    client.print(temperature);
    client.print(F("&humidity="));
    client.print(humidity);
    client.print(F("&light1="));
    client.print(pinState1);
    client.print(F("&light2="));
    client.print(pinState2);
    client.print(F("&light3="));
    client.print(pinState3);
    client.println(F(" HTTP/1.1"));

    client.println(F("Host: dweet.io"));
    client.println(F("Connection: keep-alive\r\n"));
    client.println(F(""));

    Serial.println(F("done."));
  }

  Serial.println(F(""));
}


double readFromSensor1()
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = (duration1 / 2) / 29.1;
  return distance1;
}

double readFromSensor2()
{
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  return distance2;
}

double readFromSensor3()
{
  digitalWrite(trigger3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger3, LOW);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = (duration3 / 2) / 29.1;
  return distance3;
}
