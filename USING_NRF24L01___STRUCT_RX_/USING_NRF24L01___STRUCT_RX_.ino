// http://www.bajdi.com
// Sending a struct with the nRF24L01 module
// Data to be sent is the reading of 2 analog pins
// Data received is the analog reading of 2 pins on the other Arduino

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

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

RF24 radio(8, 9);  // make sure this corresponds to the pins you are using
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL
};

void setup()
{
  Serial.begin(57600);
  radio.begin();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
}

void loop(void)
{
  // we need data to sent...
  duino1.A = analogRead(A0);
  duino1.B = analogRead(A1);
  duino1.C = analogRead(A0) / 102.3;
  duino1.D = analogRead(A1) / 102.3;
  // end of analog reading

  // radio stuff
  if ( radio.available() )
  {
    bool done = false;
    while (!done)
    {
      done = radio.read( &duino2, sizeof(duino2) );
    }
    radio.stopListening();
    radio.write( &duino1, sizeof(duino1) );
    radio.startListening();
  }
  // end of radio stuff

  // serial print received data
  Serial.print("duino2.W = ");
  Serial.println(duino2.W);
  Serial.print("duino2.X = ");
  Serial.println(duino2.X);
  Serial.print("duino2.Y = ");
  Serial.println(duino2.Y);
  Serial.print("duino2.Z = ");
  Serial.println(duino2.Z);
  // end of serial printing
}
