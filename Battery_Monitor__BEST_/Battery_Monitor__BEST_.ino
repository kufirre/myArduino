
#include <Metro.h>
#include <Streaming.h>
#include <LiquidCrystal.h>
/*
  Simple Solar Battery Monitor

  The circuit:
  Volts ( pin0) scaled input from 24vdc battery to max 5v for digitize
  Amps (pin1) input for hall effect current sensor 0 amps ~ 3.5v
  Output to 16x2 LCD display
  Metro Library for timing and Streaming for Serial output

  created Aug 2010 for Battery Monitor
  by Jerry Jeffress

*/
const int voltsInPin = 0;
const int ampsInPin = 1;
const int backLitePin = 5;
const int contrastPin = 6;
const float batterySize = 9.48;  //Battery capacity in KwHrs
const float chargeEff = 0.85;   //Approx Amount of power into a lead acid battery that is stored

Metro oneSecond = Metro(1000);  // Instantiate an one timer
int aSecond;
int blinkit;
int bStatus;  //Battery status, 0-6 states
float absorbCtr, chargeCtr, disChargeCtr, eqCtr;
float absorbTimeOut = 60 * 60 * 3.0; //3 hours in seconds
float eqTimeOut = 60 * 60 * 3.0; //3 Hours in seconds
float tenHours = 60 * 60 * 10.0;
float sec2Hr = 1.0 / (1000 * 3600.0); //Convert watt-sec to kwHrs
float volts, amps, power;
float  bCharge;  //This is power monitor variable total effective charge to and from the battery
float bLow;  //low battery value
// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600); // Start the Serial communication for debug mainly
  analogWrite(backLitePin, 64);  //LCD backlite 1/4 full 20.6 ma on battery pack
  analogWrite(contrastPin, 80); //Set LCD contrast
  aSecond = 0;
  blinkit = 0;
  bCharge = batterySize * 0.95; //init charge 95%  full battery
  absorbCtr = 0;
  eqCtr = 0;
  chargeCtr = 60 * 9;
  disChargeCtr = 0;
  bLow = 0.5;  //Low battery warning when hattery 50% charged
  bStatus = 7;//This a an error value must be updated in Calculate bStatus
  // set up the LCD's number of rows and columns:
  lcd.begin(16, 2);
};

void loop() {
  if (oneSecond.check() == 1) { // check if the metro has passed it's interval .
    //sample volt and amps from the battery
    sampleVoltsAmps();
    /* // Output serial time volts amps as to display
      Serial << aHour << _BYTE(58) << aMinute << _BYTE(58) <<
      aSecond << _BYTE(9) << volts << _BYTE(9) << amps << _BYTE(9) << bCharge << _BYTE(9) <<
      (batteryStatus
      ) << endl;
    */
    //Update LCD display each second
    calcPower();
    calcbStatus();
    newDay();

    if (aSecond % 6  < 3)
      displayStatus();
    else
      displayCtr();
    aSecond = aSecond++;
    if (aSecond >= 60)
      aSecond = 0;
    blinkit = blinkit++;
    if (blinkit > 1)
      blinkit = 0;
  }
}

void sampleVoltsAmps() {
  int voltSum = 0.0;
  float ampSum = 0.0;
  // loop 10 time over one 50hz cycle to smooth
  for (int i = 0; i < 10; i++) {
    volts = map(analogRead(voltsInPin), 0, 1023, 0, 3309);
    voltSum = voltSum + volts;
    amps = map(analogRead(ampsInPin), 0, 1023, -806, 217);
    ampSum = ampSum + amps;
    delay(2);
  }//end smooth loop
  //Get 10 sample average
  volts = voltSum / 1000.0; //Factor to scale to volts
  amps = ampSum * 0.015;  //Factor to scale to Amps
}

//Calc power
void calcPower() {
  power =  volts * amps;  //Units are watt-seconds
  if (power <= 0) {
    bCharge = bCharge + (power * sec2Hr);
  }
  else {
    bCharge = bCharge + (power * sec2Hr * chargeEff);
  }
}
//Calculate bStatus
void calcbStatus() {
  bStatus = 0; //This charge/discharge
  if (amps >= 0)
    chargeCtr = chargeCtr + 1;
  if (amps < 0) {
    disChargeCtr = disChargeCtr + 1;
  }

  //if(volts < 28.3)
  //absorbCtr = absorbCtr -1; //debug only commint
  if (absorbCtr < 0)
    absorbCtr = 0;
  if (volts > 28.3) {
    bStatus = 1;
    absorbCtr = absorbCtr + 1;
  }
  if (volts > 27.3 && absorbCtr > absorbTimeOut)
    bStatus = 2;
}
// New Day
void newDay() {
  if (disChargeCtr == tenHours) //10 hours
    chargeCtr = 0;
  if (chargeCtr > 60 * 10 && disChargeCtr > tenHours) {
    Serial << "Debug trap new day" << _BYTE(9) << disChargeCtr << endl;
    disChargeCtr = 0;
    absorbCtr = 0;
    eqCtr = 0;
    //Other saves for a New Day
  }

}
//update LCD Display
void displayStatus() {

  // Print volts to LCD.
  lcd.setCursor(0, 0);
  lcd.print("BAT ");
  lcd.print(volts, 1); //display volts to one decimal place
  lcd.print("v  ");
  // Print % full to display
  if (bCharge < batterySize) {
    lcd.print(100 * bCharge / batterySize, 1);
    lcd.print("%");
  }
  else {
    lcd.print("Full ");
    bCharge = batterySize;
  }
  //Print Battery Status to Display
  lcd.setCursor(0, 1);
  switch (bStatus) {
    case 0:
      if (amps >= 0)
        lcd.print("Charge   ");
      else
        lcd.print("Discharge ");
      break;
    case 1:
      lcd.print("Absorb   ");
      break;
    case 2:
      lcd.print("Float    ");
      break;
    default:
      lcd.print("Error    ");
  }
  if (bCharge < bCharge * bLow) { //Battery Low blink
    if (blinkit == 0)
      lcd.print("Low      ");
    else
      lcd.print("      Low");
  }
  if (volts > 29.2 && eqCtr < eqTimeOut) {
    lcd.setCursor(0, 1);
    lcd.print("Equalize ");
    eqCtr = eqCtr + 1;
  }
  //Print Amps to display right justified
  if (amps < 0) { //All negative
    if (amps < -9.9) {
      lcd.print(amps, 1);
    }
    else {
      lcd.print(" ");
      lcd.print(amps, 1);
    }
  }
  else { //all positive or zero
    if (amps > 9.9) {
      lcd.print("+");
      lcd.print(amps, 1);
    }
    else {
      lcd.print(" +");
      lcd.print(amps, 1);
    }
  }

  lcd.print(" A");
}

//Counter page
void displayCtr() {
  lcd.clear();
  lcd.print("Chr ");
  lcd.print(chargeCtr / 60, 0);
  lcd.print("  Dis ");
  lcd.print(disChargeCtr / 60, 0
           );
  lcd.setCursor(0, 1);
  lcd.print("Abs ");
  lcd.print(absorbCtr / 60, 0);
  lcd.print("   eq ");
  lcd.print(eqCtr / 60, 0);
}
