
void setup()
{
  Serial.begin(9600);
  Serial.println("MASTER(HC-05):");

  // HC-06 default baud rate is 9600
  Serial1.begin(9600);
}

void loop()
{

  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (Serial1.available())
    Serial.write(Serial1.read());

  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
    Serial1.write(Serial.read());
}
