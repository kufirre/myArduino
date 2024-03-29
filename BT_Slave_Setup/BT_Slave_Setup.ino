#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); // RX | TX

void setup()
{
  Serial.begin(9600);
  Serial.println("SLAVE(HC-06):");
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  // HC-06 default baud rate is 9600
  BTSerial.begin(9600);
}

void loop()
{

  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
    BTSerial.write(Serial.read());
}
