// http://www.bajdi.com
// Sending a struct with the nRF24L01 module
// Data to be sent is the reading of 2 analog pins
// Data received is the analog reading of 2 pins on the other Arduino

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(1, 2); // make sure this corresponds to the pins you are using
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL
};

const int led = 0;

typedef struct {
  int A;
  int B;
  float C;
  float D;
}
A_t;

typedef struct {
  int W;
  int X;
  float Y;
  float Z;
}
B_t;

A_t duino1;
B_t duino2;

void setup()
{
  Serial.begin(57600);
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
}

void loop(void)
{
  // we need data to sent...
  duino2.W = analogRead(A0);
  duino2.X = analogRead(A1);
  duino2.Y = analogRead(A0) / 102.3;
  duino2.Z = analogRead(A1) / 102.3;
  // end of analog reading

  // radio stuff
  radio.stopListening();
  bool ok = radio.write( &duino2, sizeof(duino2) );
  radio.startListening();

  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 250 )
      timeout = true;

  if ( timeout )
  {
    Serial.println("Failed, response timed out.");
    digitalWrite(led, HIGH);
  }
  else
  {
    radio.read( &duino1, sizeof(duino1) );
    digitalWrite(led, LOW);
  }
  // end of radio stuff

  // serial print received data
  Serial.print("duino1.A = ");
  Serial.println(duino1.A);
  Serial.print("duino1.B = ");
  Serial.println(duino1.B);
  Serial.print("duino1.C = ");
  Serial.println(duino1.C);
  Serial.print("duino1.D = ");
  Serial.println(duino1.D);
  // end of serial printing
}
