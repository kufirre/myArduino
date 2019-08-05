#define SERVO_PIN         7  // Any pin on the Arduino or Mega will work.

void setup()
{
  pinMode(SERVO_PIN, OUTPUT);

}

int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
float lenMicroSecondsOfPulse = 2.0 * 1000; // 0.5 ms is 0 degrees while 1.8  is 180 degrees

void loop()
{

 // Servos work by sending a 20 ms pulse.  
 // 1.0 ms at the start of the pulse will turn the servo to the 0 degree position
 // 1.5 ms at the start of the pulse will turn the servo to the 90 degree position 
 // 2.0 ms at the start of the pulse will turn the servo to the 180 degree position 
 // Turn voltage high to start the period and pulse
 digitalWrite(SERVO_PIN, HIGH);

 // Delay for the length of the pulse
 delayMicroseconds(lenMicroSecondsOfPulse);

 // Turn the voltage low for the remainder of the pulse
 digitalWrite(SERVO_PIN, LOW);

 // Delay this loop for the remainder of the period so we don't
 // send the next signal too soon or too late
 delayMicroseconds(lenMicroSecondsOfPeriod - lenMicroSecondsOfPulse); 

}
