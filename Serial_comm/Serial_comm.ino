#include <SoftwareSerial.h>

SoftwareSerial gsm(3, 2);


void setup() {  // put your setup code here, to run once:
  gsm.begin(9600);

  Serial.begin(9600);

}

void loop() {
  if (gsm.available()) {
    Serial.print(char(gsm.read()));
  }

  while (Serial.available()) {
    gsm.write(char(Serial.read()));
  }
}
