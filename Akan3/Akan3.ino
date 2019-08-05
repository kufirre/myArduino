#include<dht.h>
dht DHT; 
#define DHT11_PIN 13
#define dHTH_HUM_CHANNEL 2
#define dHTT_TEMP_CHANNEL 15

#include <Wire.h>
#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);


long distance1, distance2, distance3, duration1, duration2, duration3;

const int light1 = 3;
const int light2 = 4;
const int light3 = 5;
const int light4 = 12;

const int trigger1 = 11;
const int trigger2 = 9;
const int trigger3 = 7;

const int echo1 = 10;
const int echo2 = 8;
const int echo3 = 6;

int hour, minute, second;

void setup()
{
  Serial.begin(115200);
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
  pinMode(light4, OUTPUT);

  // Set ESP8266 baud rate
  delay(10);

  Wire.begin();

  
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  int temp = DHT.temperature;
  int humid = DHT .humidity;

  readFromSensor1();
  readFromSensor2();
  readFromSensor3();

  getTime();

  if ( (hour == 13) && (minute == 00) && (second == 00) ) //This determines the time when the ultrasonic sensors start functioning
  {
    if (distance1 >= 5)
      digitalWrite(light1, HIGH);

    if (distance2 > 10)
      digitalWrite(light2, HIGH);

    if (distance3 > 15)
      digitalWrite(light3, HIGH);
  }
  Cayenne.loop();
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

CAYENNE_OUT(dHTT_CHANNEL)
{
  // Read data from the sensor and send it to the virtual channel here.
  // You can write data using virtualWrite or other Cayenne write functions.
  // For example, to send a temperature in Celsius you can use the following:
  //Cayenne.virtualWrite(VIRTUAL_CHANNEL,25.5,TYPE_TEMPERATURE, UNIT_CELSIUS);
  DHT.read11(DHT11_PIN);
  int temp = DHT.temperature;

  Cayenne.virtualWrite(dHTT_TEMP_CHANNEL, temp, TYPE_TEMPERATURE, UNIT_CELSIUS);
}

CAYENNE_OUT(dHTH_CHANNEL)
{
  // Read data from the sensor and send it to the virtual channel here.
  // You can write data using virtualWrite or other Cayenne write functions.
  // For example, to send a temperature in Celsius you can use the following:
  //Cayenne.virtualWrite(VIRTUAL_CHANNEL,25.5,TYPE_TEMPERATURE, UNIT_CELSIUS);
  DHT.read11(DHT11_PIN);
  int humid = DHT.humidity;
  Cayenne.virtualWrite(dHTH_HUM_CHANNEL, humid, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
  //Cayenne.virtualWrite(dHTH_CHANNEL,analogRead(DHT.humidity));
}

CAYENNE_IN(light1_channel)
{
  // Write value to turn the relay switch on or off. This code assumes you wire your relay as normally open.

  if (!getValue.asInt() == 0)
  {
    digitalWrite(light1, HIGH);
  }
  else
  {
    digitalWrite(light1, LOW);
  }
}

CAYENNE_IN(light2_channel)
{
  // Write value to turn the relay switch on or off. This code assumes you wire your relay as normally open.

  if (!getValue.asInt() == 0)
  {
    digitalWrite(light2, HIGH);
  }
  else
  {
    digitalWrite(light2, LOW);
  }
}

CAYENNE_IN(light3_channel)
{
  // Write value to turn the relay switch on or off. This code assumes you wire your relay as normally open.

  if (!getValue.asInt() == 0)
  {
    digitalWrite(light3, HIGH);
  }
  else
  {
    digitalWrite(light3, LOW);
  }
}

/*CAYENNE_IN(light4_channel)
{
  // Write value to turn the relay switch on or off. This code assumes you wire your relay as normally open.

  //if (!getValue.asInt() == 0) //Read int value from channel4 and if it's not zero
  {
   // digitalWrite(light4, HIGH);
  }
  else
  {
    digitalWrite(light4, LOW);
  }
}*/

int getTime()
{
  DateTime now = RTC.now();

  //Serial.print(now.hour(), DEC);

  Serial.print(':');

  //Serial.print(now.minute(), DEC);

  Serial.print(':');

  //Serial.print(now.second(), DEC);

  //Serial.println();

  delay(10);
}
