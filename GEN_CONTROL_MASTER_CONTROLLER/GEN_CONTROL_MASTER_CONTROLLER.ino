// http://www.bajdi.com
// Sending a struct with the nRF24L01 module
// Data to be sent is the reading of 2 analog pins
// Data received is the analog reading of 2 pins on the other Arduino

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

void setup()
{
  Serial.begin(9600);

  // HC-06 default baud rate is 9600, connected to the hardware serial1
  Serial1.begin(9600);
}

void loop()
{

  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (Serial1.available())
    Serial.println(Serial1.read());

  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
    Serial1.write(Serial.read());
}
