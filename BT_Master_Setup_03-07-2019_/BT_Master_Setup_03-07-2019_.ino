//char string[32];
char string;
String byteRead = "";


void setup()
{
  Serial.begin(9600);
  Serial.println("MASTER(HC-05):");

  // HC-06 default baud rate is 9600
  Serial1.begin(9600);
}

void loop()
{
  byteRead = "";
  while (Serial1.available() > 0) {
    string = Serial1.read();
    byteRead += string;
  }
  Serial.println(byteRead.toInt() + 200);
  delay(300);
  
}
