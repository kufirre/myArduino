int presetPin = 24;
int resetPin = 25;
int goSwitch = 23;
int carryPin = 22;

int carryTimes = 0; //variable to check the number of times the units 7-segment goes to zero

//traffic light pins
int redLed = 26;
int greenLed = 27;

bool carryState = false;

void setup() {

  pinMode(presetPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(goSwitch, OUTPUT);
  pinMode(carryPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(presetPin, HIGH);
  delay(100);
  digitalWrite(presetPin, LOW);
  digitalWrite(resetPin, LOW);

}

void loop() {

  digitalWrite(goSwitch, LOW);              // start the countdown
  carryState =  digitalRead(carryPin);      // read the state of the carry pin
  digitalWrite(redLed, HIGH);         // turn on the redLed

  if (carryState == LOW)              // check to see if the carryPin is giving 0V
  {
    carryTimes++;                     // increment the number of times the units 7-segment has gotten to zero
  }

  if (carryTimes == 6) {              // if number of times the units 7-segment has gotten to zero, initialize the parameters
    digitalWrite(goSwitch, HIGH);     // stop the countdown
    digitalWrite(redLed, LOW);        // turn off the redLed
    digitalWrite(greenLed, HIGH);      // turn on the greenLed

    // reset
    digitalWrite(resetPin, HIGH);

    carryTimes = 0;                   // set the carryTimes variable back to zero
    delay(2000);                      // wait for 2 seconds before resetting

    // reset the count
    digitalWrite(resetPin, LOW);
    digitalWrite(presetPin, HIGH);
    delay(10);
    digitalWrite(presetPin, LOW);
  }

  else {
    digitalWrite(goSwitch, LOW);
  }


  delay (1000);                        // delay for one second before starting again: this is where you start the other seven-segment 
  digitalWrite(greenLed, LOW);         // turn off the greenLed
}
