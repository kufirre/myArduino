#include "sha1.h"

void setup() {
  uint8_t* hash;
  uint32_t a;

  pinMode(12, OUTPUT);

  Serial.begin(9600);

  // SHA tests
  Serial.println("Test: FIPS 180-2 C.1 and RFC3174 7.3 TEST1");
  Serial.println("Expect:5abf7f38f3cc24551158fe362372ca268f947f02"); //5abf7f38f3cc24551158fe362372ca268f947f02
  Serial.print("Result:"); 
  Sha1.init();
  Sha1.print("newlevel");
  printHash(Sha1.result());
  Serial.println();

}

void loop(){
  
}



void printHash(uint8_t* hash) {
  int i;
  for (i = 0; i < 20; i++) {
    Serial.print("0123456789abcdef"[hash[i] >> 4]);
    Serial.print("0123456789abcdef"[hash[i] & 0xf]);
  }
  Serial.println();
}
