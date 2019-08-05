#include <SoftwareSerial.h>
//#include <NeoSWSerial.h>

//NeoSWSerial A6MODULE(3, 2);

//SoftwareSerial A6MODULE(2, 3);

void setup() {  // put your setup code here, to run once:
  Serial1.begin(9600);
//  delay(1000);
  Serial.begin(9600);
//  delay(1000);
}

void loop() {
  while (Serial1.available()) {
    Serial.print(char(Serial1.read()));
  }

  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
