#include <avr/pgmspace.h>
#include <sha1.h>

char key[] = "newlevel";
uint8_t basestring[] = { 0x74, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67 }; // testing

void printHash(uint8_t* hash) {  
  for (int i=0; i<20; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  Serial.print("Input:              ");
  for (int i=0; i<sizeof(basestring); i++) {
    Serial.print((char) basestring[i]);
  }
  Serial.println();

  Serial.print("Key:                ");
  Serial.println(key);

  Serial.print("Hmac-sha1 (hex):    ");
  Sha1.initHmac((uint8_t*)key, strlen(key));

  for (int i=0; i<sizeof(basestring); i++) {
    Sha1.print((char) basestring[i]);
  }

  uint8_t *hash;
  hash = Sha1.resultHmac();
  printHash(hash);

}

void loop() { }
