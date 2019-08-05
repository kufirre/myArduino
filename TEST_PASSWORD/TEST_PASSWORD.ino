/*
   Code by Kufirre Ebong: kuffykeys@gmail.com
   01/01/2019
   ALWAYS REMEMBER to use flags to for states and transitions in your code
*/


#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>


char password[5];
char initial_password[4], new_password[4];
int i = 0;
int relay_pin = 10;
char key_pressed = 0;

const byte rows = 4;
const byte columns = 4;

bool voltLowerLimit = false;
bool voltUpperLimit = false;
bool chargeUpperLimit = false;
bool currentCharge = false;
bool fuelUpperLimit = false;
bool fuelLowerLimit = false;

bool pswd = false;


char hexaKeys[rows][columns] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

byte column_pins[rows] = {9, 8, 7, 6};
byte row_pins[columns] = {5, 4, 3, 2};

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup()


/*
   this should be the first instance a password is requested for.
   after this, the DEFAULT STATE OF THE SWITCH STATEMENT should run until key is pressed
*/
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("POWER PROJECT");
  lcd.setCursor(0, 1);
  //  initialpassword();
  keypad_key.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad_key.setHoldTime(5000); //set the amount of milliseconds the user will have to hold a button until the HOLD state is triggered. Default is 1000ms
}



void loop()
{

  digitalWrite(13, LOW);
  key_pressed = keypad_key.getKey();

  switch (key_pressed)
  {
    case '#':
      change();
      break;

    case 'C':
      for (i = 0; i < 5; i++)
      {
        password[i++] = "\0";
      }
      i = 0;
      lcd.clear();
      lcd.print("CANCELED");
      lcd.setCursor(0, 1);
      break;

    case 'A':
      /*
         this should change the input parameters for the fuel
         it should first demand for the password
         The first value to be set should be the "TANK EMPTY VALUE(CM)" or press "A" for 4 seconds to use the current value of battery FROM ULTRASONIC SENSOR
         afterwards, the "ENTER KEY"(which is "A" in our case) should be pressed   to store this value
         it then writes this value to an EEPROM address using a for loop for each of the characcters of the value
         it then demands for "TANK FULL VALUE(CM)" or press "A" for 4 seconds to use the current value of battery FROM ULTRASONIC SENSOR
         afterwards, the "ENTER KEY"(which is "A" in our case) should be pressed to store this value
         it then writes this value to an EEPROM address using a for loop for each of the characcters of the value
         values are saved and it returns to HOME SCREEN
      */

      bool pswd = true;
      if (pswd)
        requestPassword();

      else
        pswd = false;

      voltLowerLimit = false;
      voltUpperLimit = false;
      chargeUpperLimit = false;
      currentCharge = false;
      fuelUpperLimit = false;
      fuelLowerLimit = true;

      break;



    case 'B':
      /*
         set battery capacity (BAT FULL CHARGE(mAh)) or press "A" for 4 seconds to use the current value of battery FROM CURRENT SENSOR
         set current battery capacity (BAT CHARGE(mAh)) or press "A" for 4 seconds to use the current value of battery FROM CURRENT SENSOR
      */
      voltLowerLimit = false;
      voltUpperLimit = false;
      chargeUpperLimit = true;
      currentCharge = false;
      fuelUpperLimit = false;
      fuelLowerLimit = false;
      break;





    case 'D':
      /*
         set battery voltage parameters
         the first value should be the "VOLTS OF FULL BAT(V)" or press "A" for 4 seconds to use the current value of battery FROM VOLTAGE SENSOR
         the second value should be the "VOLTS OF LOW BAT(V)" or press "A" for 4 seconds to use the current value of battery FROM VOLTAGE SENSOR
         use this value to callibrate the voltage to percentage
         send a signal to the ATS to transfer the LOAD and another signal to START/STOP THE GEN based on this parameter, combined with the BAT CHARGE
      */
      voltLowerLimit = true;
      voltUpperLimit = false;
      chargeUpperLimit = false;
      currentCharge = false;
      fuelUpperLimit = false;
      fuelLowerLimit = false;

      break;


    default:
      /*
         this should contain HOME SCREEN parameters which include:
         1. battery  charge
         2. battery discharge
         3. battery voltage
         4. battery temperature
         5.
      */
      if (key_pressed && key_pressed != 'C')
      {
        password[i++] = key_pressed;
        lcd.print(key_pressed);
      }
      break;
  }




  if (i == 4)
  {
    delay(200);
    for (int j = 0; j < 4; j++)
      initial_password[j] = EEPROM.read(j);

    if (!(strncmp(password, initial_password, 4)))
    {
      lcd.clear();
      lcd.print("Pass Accepted");
      digitalWrite(13, HIGH);

      delay(1500);

      lcd.setCursor(0, 1);
      lcd.print("Press # to change");
      delay(1000);

      lcd.clear();
      lcd.print("Enter Password:");

      lcd.setCursor(0, 1);
      i = 0;
    }

    else
    {
      digitalWrite(13, LOW);
      lcd.clear();
      lcd.print("Wrong Password");
      lcd.setCursor(0, 1);
      lcd.print("Press # to Change");
      delay(1500);
      lcd.clear();
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      i = 0;
    }
  }
}
}


//  function to change password
void change()

{
  int j = 0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0, 1);
  while (j < 4)
  {
    char key = keypad_key.getKey();
    if (key) // if a key is pressed
    {
      new_password[j++] = key; //tries to get the current 4 digit password the user is typing
      lcd.print(key);
    }
    key = 0; // reset the key
  }

  delay(500);

  if ((strncmp(new_password, initial_password, 4))) // compare the inputed password with the initially set password
  {
    lcd.clear();
    lcd.print("WRONG Password"); //  this function enables one set password
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(1000);
  }

  else // if the new password is not the initial password
  {
    j = 0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0, 1);
    while (j < 4)

    {
      char key = keypad_key.getKey();
      if (key)
      {
        initial_password[j] = key;
        lcd.print(key);
        EEPROM.write(j, key);
        j++;
      }

    }
    lcd.setCursor(0, 2);
    lcd.print("Pass Changed");
    delay(1000);
  }

  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  key_pressed = 0;
}


// this function sets initial password to "1234"
void initialpassword() {
  for (int j = 0; j < 4; j++)

    // default password is set to 49, which is the ASCII character for one
    EEPROM.write(j, j + 49); //increment from 1 to 4 and write to addresses 0 - 3

  for (int j = 0; j < 4; j++)
    initial_password[j] = EEPROM.read(j); // read the initial password values from EEPROM addresses 0 - 3
}




void keypadEvent(KeypadEvent key_pressed) {
  switch (keypad_key.getState()) {

    case HOLD:
      if (key_pressed == '*' && voltLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET");
        delay(1000);
        voltLowerLimit = false;
      }

      if (key_pressed == '*' && voltLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET");
        delay(1000);
        voltLowerLimit = false;
      }

      if (key_pressed == '*' && chargeUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("CHARGE UPPER LIMIT SET");
        delay(1000);
        chargeUpperLimit = false;
      }

      if (key_pressed == '*' && fuelLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("FUEL LOWER LIMIT SET");
        delay(1000);
        fuelLowerLimit = false;
      }

      break;
  }
}

bool requestPassword()
{
  {
    int j = 0;
    lcd.clear();
    lcd.print("Password:");
    lcd.setCursor(0, 1);
    while (j < 4)
    {
      char key = keypad_key.getKey();
      if (key) // if a key is pressed
      {
        new_password[j++] = key; //tries to get the current 4 digit password the user is typing
        lcd.print(key);
      }
      key = 0; // reset the key
    }

    delay(500);

    if ((strncmp(new_password, initial_password, 4))) // compare the inputed password with the initially set password
    {
      lcd.clear();
      lcd.print("PERMISSION GRANTED"); //  this function enables one set password
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("Set *** parameter"); // SET LOWER AND UPPER LIMIT OF PARAMETERS OR ASSIGN THEM IF HOLD KEY
      delay(1000);
      /*
         Set the new parameter here and write it to the EEPROM immediately
         remember the to leave enough gaps between the addresses
         Should this function be here or in the SWITCH-CASE statement????
         Let's test it first then decide later
      */
      //    lcd.setCursor(0, 1);
      //    while (j < 4)
      //
      //    {
      //      char key = keypad_key.getKey();
      //      if (key)
      //      {
      //        initial_password[j] = key;
      //        lcd.print(key);
      //        EEPROM.write(j, key);
      //        j++;
      //      }
      //
      //    }
      //    lcd.setCursor(0, 2);
      //    lcd.print("Pass Changed");
      //    delay(1000);
      return true;
    }

    else // if the new password is not the initial password
    {
      j = 0;
      lcd.clear();
      lcd.print("PERMISSION DENIED!");
      return false;

    }

    lcd.clear();
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    key_pressed = 0;
  }
}
