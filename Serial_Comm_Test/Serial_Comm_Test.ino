//#include "SoftwareSerial.h"
#include <NeoSWSerial.h>
//#include <AltSoftSerial.h>

int relay = 5;
//SoftwareSerial soft(8,9); // RX, TX
NeoSWSerial soft(9,10);
//AltSoftSerial soft;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    soft.begin(9600);
  pinMode(relay, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //  digitalWrite(relay, LOW);
  //  delay(200);
  //  digitalWrite(relay, HIGH);
  //  delay(200);
  while (soft.available() > 0)
  {
    char data = soft.read();
    Serial.println(data);
  }
}
