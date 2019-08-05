#include <SoftwareSerial.h>
SoftwareSerial myGsm(10, 11);

void setup()
{
  myGsm.begin(9600);
  Serial.begin(9600);

  Serial.println("Connection");
  delay(2000);
  Serial.println("Complete!...");
  myGsm.flush();
  Serial.flush();


  // Cheking to see if gprs is attached
  myGsm.println("AT+CGATT?");
  delay(1000);
  PrintSerialData();

  // Check signal strength
  myGsm.println("AT+CSQ ");
  delay(1000);
  PrintSerialData();

  // Set connection type to GPRS
  myGsm.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(2000);
  PrintSerialData();

  // Set the APN
  myGsm.println("AT+SAPBR=3,1,\"APN\",\"fast.t-mobile.com\"");
  delay(2000);
  PrintSerialData();

  // Enable GPRS
  myGsm.println("AT+SAPBR=1,1");
  delay(10000);
  PrintSerialData();

  // Check to see if connection is correct and get your IP address
  myGsm.println("AT+SAPBR=2,1");
  delay(2000);
  PrintSerialData();

}


void loop()
{

  float x=10;
  float y=11;
   // Start http service
   myGsm.println("AT+HTTPINIT");
   delay(2000); 
   PrintSerialData();

   // Set value
   
   myGsm.println(String("AT+HTTPPARA=\"URL\",\"http://www.dweet.io/dweet/for/MYthingname?lat="+ String(x)+"&lon="+ String(y)+"\""));
   delay(4000);
   PrintSerialData();

   // set http action type 0 = GET
   myGsm.println("AT+HTTPACTION=0");
   delay(6000);
   PrintSerialData();

   // read server response
   myGsm.println("AT+HTTPREAD"); 
   delay(1000);
   PrintSerialData();

   //myGsm.println("");
   myGsm.println("AT+HTTPTERM");
   PrintSerialData();
   delay(300);

   myGsm.println("");
   delay(10000);
}

void PrintSerialData()
{
  while(myGsm.available()!=0)
  {
    Serial.write(myGsm.read());
  }
}
