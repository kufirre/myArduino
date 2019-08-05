// pins for the green 7-segment
int presetPinGreen = 52;
int resetPinGreen = 51;
int goSwitchGreen = 53;
int carryPinGreen = 50;

int carryTimesGreen = 0;
int carryStateGreen;

//pins for the Red 7-segment
int presetPinRed = 24;
int resetPinRed = 25;
int goSwitchRed = 23;
int carryPinRed = 22;

int carryTimesRed = 0;
int carryStateRed;

//traffic light pins
int redLed1 = 26;
int greenLed1 = 27;
int redLed3 = 29;
int greenLed3 = 28;
int redLed2 = 49;
int greenLed2 = 48;

void setup() {

pinMode(presetPinGreen, OUTPUT);
pinMode(resetPinGreen, OUTPUT);
pinMode(goSwitchGreen, OUTPUT);
pinMode(carryPinGreen, INPUT);

pinMode(presetPinRed, OUTPUT);
pinMode(resetPinRed, OUTPUT);
pinMode(goSwitchRed, OUTPUT);
pinMode(carryPinRed, INPUT);

pinMode(redLed3, OUTPUT);
pinMode(greenLed3, OUTPUT);
pinMode(redLed1, OUTPUT);
pinMode(greenLed1, OUTPUT);

digitalWrite(goSwitchRed, HIGH);
digitalWrite(goSwitchGreen, HIGH);
digitalWrite(presetPinGreen, LOW);
digitalWrite(resetPinGreen, LOW);
digitalWrite(presetPinRed, LOW);
digitalWrite(resetPinRed, LOW);
digitalWrite(redLed3, LOW);
digitalWrite(greenLed3, LOW);
digitalWrite(redLed1, LOW);
digitalWrite(greenLed1, LOW);
}
void loop() {
  //FIRST SET OF TRAFFIC LIGHTS
digitalWrite(redLed1, HIGH);
digitalWrite(redLed3, HIGH);
digitalWrite(greenLed1, LOW);
digitalWrite(greenLed3, LOW);
redSegment();
  delay(200);
digitalWrite(redLed1, LOW);
digitalWrite(redLed3, HIGH);
digitalWrite(greenLed1, HIGH);
redSegment();
  delay(200);
digitalWrite(greenLed1, LOW);
digitalWrite(redLed1, HIGH);
digitalWrite(redLed3, HIGH);
redSegment();
  delay(200);
digitalWrite(greenLed3, HIGH);
digitalWrite(redLed1, HIGH);
digitalWrite(redLed3, LOW);
redSegment();
  delay(100);

//  //SECOND SET OF TRAFFIC LIGHTS
//  digitalWrite(greenLed2, HIGH);
//  greenSegment();
//  delay(1000);
//  digitalWrite(greenLed2, LOW);
//  digitalWrite(redLed2, HIGH);
//  redSegment();
//  delay(1000);
//  digitalWrite(redLed2, LOW);
//  
}
// this is for the red 7-segment
void redSegment() {

carryTimesRed = 0; //variable to check the number of times the units 7-segment goes to zero

carryStateRed = digitalRead(carryPinRed);      // read the state of the carry pin

digitalWrite(resetPinRed, LOW);
digitalWrite(goSwitchGreen, HIGH);
digitalWrite(presetPinRed, HIGH);
  delay(100);
digitalWrite(presetPinRed, LOW);

  while (carryTimesRed< 6) {

digitalWrite(goSwitchRed, LOW);              // start the countdown
digitalWrite(goSwitchGreen, HIGH);

carryStateRed = digitalRead(carryPinRed);      // read the state of the carry pin

    if (carryStateRed == LOW &&carryTimesRed != 6)              // check to see if the carryPin is giving 0V
    {
carryTimesRed++;                     // increment the number of times the units 7-segment has gotten to zero
      delay(300);
    }

    if (carryTimesRed == 6) {              // if number of times the units 7-segment has gotten to zero, initialize the parameters
digitalWrite(goSwitchRed, HIGH);     // stop the countdown

      // reset
digitalWrite(resetPinRed, HIGH);
      delay(100);
digitalWrite(resetPinRed, LOW);

    }

  }
}


// this is for the red 7-segment
void greenSegment() {

carryTimesGreen = 0; //variable to check the number of times the units 7-segment goes to zero

carryStateGreen = digitalRead(carryPinGreen);      // read the state of the carry pin

digitalWrite(resetPinGreen, LOW);
digitalWrite(goSwitchRed, HIGH);
digitalWrite(presetPinGreen, HIGH);
  delay(100);
digitalWrite(presetPinGreen, LOW);



  while (carryTimesGreen< 6) {

    //    digitalWrite(greenLed2, HIGH);         // turn on the redLed
digitalWrite(goSwitchGreen, LOW);              // start the countdown
digitalWrite(goSwitchRed, HIGH);

carryStateGreen = digitalRead(carryPinGreen);      // read the state of the carry pin

    if (carryStateGreen == LOW &&carryTimesGreen != 6)              // check to see if the carryPin is giving 0V
    {
carryTimesGreen++;                     // increment the number of times the units 7-segment has gotten to zero
      delay(300);
    }

    if (carryTimesGreen == 6) {              // if number of times the units 7-segment has gotten to zero, initialize the parameters
digitalWrite(goSwitchGreen, HIGH);     // stop the countdown

      // reset
digitalWrite(resetPinGreen, HIGH);
      delay(100);
digitalWrite(resetPinGreen, LOW);

    }

  }

}
