/*
   Code by Kufirre Ebong: kuffykeys@gmail.com
   01/01/2019
   ALWAYS REMEMBER to:
   1. use flags to for states and transitions in your code
   2. set key presses to zero after using them
   3. initialize all parameters after using them
   4. increment the key presses in the while loop else you have an infinite loop
   5. CORRECT THE DELAYS WHEN SAMPLING CURRENT FOR THE COULOMB COUNTING PROCEDURE
   6. always remember to terminate arrays with a NULL character '\0'

*/


#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>


char password[5];
char initial_password[5], new_password[5], lower_limit[4], upper_limit[3];


int i = 0;
int a = 0;
int e = 70; // start address for fuel lowerLimit
int f = 60; // start address for fuel upperLimit


char key_pressed = 0;

const byte rows = 4;
const byte columns = 4;

bool voltLowerLimit = false;
bool voltUpperLimit = false;
bool chargeUpperLimit = false;
bool currentCharge = false;
bool fuelUpperLimit = false;
bool fuelLowerLimit = false;

bool holdFlag = false;


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

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("WELCOME.");
  delay(1500);
  lcd.setCursor(5, 0);
  lcd.print("WELCOME..");
  delay(1500);
  lcd.setCursor(5, 0);
  lcd.print("WELCOME...");
  delay(1500);

  keypad_key.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad_key.setHoldTime(5000); //set the amount of milliseconds the user will have to hold a button until the HOLD state is triggered. Default is 1000ms
  while (!requestPassword());
}



void loop()
{
  key_pressed = keypad_key.getKey();

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("HOME SCREEN");
  delay(300);

  switch (key_pressed)
  {
    case '#':
      change();
      break;

    case 'C':
      for (i = 0; i < 5; i++)
      {
        password[i++] = '\0';
      }
      i = 0;
      lcd.clear();
      lcd.print("Enter Password:");
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

         PRINT PARAMETERS FOR 1 SECOND BEFORE RETURNING TO HOME SCREEN BASED ON CALIBRATION:
         i.e. 267cm = 0%
              7     = 100%
          for fuel level measurement.


         REMEMBER TO WRITE FUNTION TO CALLIBRATE TO % VALUE FROM THE LIMITS
      */
      if (requestPassword() == true)
      {
        voltLowerLimit = false;
        voltUpperLimit = false;
        chargeUpperLimit = false;
        currentCharge = false;
        fuelUpperLimit = false;
        fuelLowerLimit = true;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Set fuel lower limit"); // SET LOWER AND UPPER LIMIT OF PARAMETERS OR ASSIGN THEM IF HOLD KEY
        lcd.setCursor(0, 2);
        delay(1000);

        //lower limit should be a 2 or 3-digit value
        int a = 0;
        int e = 70;
        int m = 0;

        while (a < 3)

        {
          char lowerLimit = keypad_key.getKey();
          if (lowerLimit && lowerLimit != '*' && lowerLimit != 'C')
          {
            lower_limit[m] = lowerLimit;
            lcd.print(lowerLimit);
            EEPROM.update(e, lowerLimit); // start the EEPROM address from e which is 70 and max is 73
            e++;
            a++;
            m++;
          }
          if (lowerLimit == '*')
          {
            a = 3;
            lower_limit[3] = '\0';
            lcd.setCursor(0, 2);
            lcd.print("Fuel LL Accepted");
          }

          //Add if condition for C, ALSO print the value stored in EEPROM
          if (lowerLimit == 'C')
          {
            for (a = 0; a < 5; a++)
            {
              lower_limit[a++] = '\0'; // clear value
            }
            a = 0;
            lcd.clear();
            lcd.print("CANCELED");
            lcd.setCursor(0, 1);
            break;
          }

/*
 * REMEMBER TO COME BACK HERE TO WRITE THE VALUE OF THE ULTRASONIC SENSOR WHEN "#" KEY IS HELD FOR SOME TIME 
 */
          if (holdFlag == true)
          {
//            for (a = 0; a < 3; a++)
//            {
//              EEPROM.update(e, 0); // clear the lower_limit address in order to write a new value to it 
//              EEPROM.update(e, ultrasonic sensor value); // write ultrasonic sensor value to it here. EDIT THIS SOON
//            }
            break;
          }
          lowerLimit = 0;
        }

        voltLowerLimit = false;
        voltUpperLimit = false;
        chargeUpperLimit = false;
        currentCharge = false;
        fuelUpperLimit = true;
        fuelLowerLimit = false;

        a = 0;  // reset the value of a here in other to start the loop for fuel upper limit
        m = 0;

        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Set fuel upper limit");
        lcd.setCursor(0, 2);

        //upper limit should be a 1 or 2-digit value
        while (a < 2)

        {
          char upperLimit = keypad_key.getKey();
          if (upperLimit && upperLimit != '*' && upperLimit != 'C')
          {
            // continue address from position 14
            upper_limit[m] = upperLimit;
            lcd.print(upperLimit);
            EEPROM.update(f, upperLimit); // start the EEPROM address from e which is 10 and max is 13
            f++;
            a++;
            m++;
          }
          if (upperLimit == '*')
          {
            if (a == 1)
            {
              for (int j = 1; j < 3; j++)
              {
                EEPROM.update((j + 60), 0);
              }

              lcd.setCursor(0, 2);
              lcd.print("Fuel UL Accepted");
            }

            if (a == 0)
            {
              for (int j = 0; j < 3; j++)
              {
                upper_limit[j++] = '\0'; // clear value
              }
              lcd.setCursor(0, 2);
              lcd.print("NOTHING ENTERED!!!");
              delay(500);
              //              break;
            }

            upperLimit = 0;
            break;
          }


          if (upperLimit == 'C')
          {
            for (a = 0; a < 5; a++)
            {
              upper_limit[a++] = '\0'; // clear value
            }
            a = 0;
            lcd.clear();
            lcd.print("CANCELED");
            lcd.setCursor(0, 1);
            break;
          }
          upperLimit = 0;
        }

        // read the lower_limit from EEPROM
        e = 70;
        delay(200);
        for (int j = 0; j < 3; j++)
        {
          lower_limit[j] = EEPROM.read(e);
          e++;
        }
        lower_limit[3] = '\0'; // set the index 3 of the lower_limit to NULL

        // read the upper_limit from EEPROM
        f = 60;
        delay(200);
        for (int j = 0; j < 2; j++)
        {
          upper_limit[j] = EEPROM.read(f);
          f++;
        }
        upper_limit[2] = '\0'; // set the index 2 of the upper_limit to NULL



        voltLowerLimit = false;
        voltUpperLimit = false;
        chargeUpperLimit = false;
        currentCharge = false;
        fuelUpperLimit = false;
        fuelLowerLimit = false;

        delay(1000);
        a = 0;

        e = 0;
        f = 0;

        lcd.clear();
        //        lcd.print("Parameters SAVED");
        //        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("upper: ");
        lcd.setCursor(7, 1);
        lcd.print(upper_limit);
        lcd.setCursor(0, 2);
        lcd.print("lower: ");
        lcd.setCursor(7, 2);
        lcd.print(lower_limit);
        Serial.println("upperLimit is: " + String(upper_limit) + " while lowerLimit is: " + String(lower_limit));
        delay(3000);
      }

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("HOME SCREEN"); //supposed to show home screen here...


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
  }
}


bool requestPassword()
{
  int x = 100;
  int j = 0;


  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(15, 0);
  delay(200);

  while (j < 4)
  {

    char checKey = keypad_key.getKey();
    if (checKey && checKey != 'C')
    {
      password[i++] = checKey;
      lcd.print(checKey);
      j++;
      checKey = 0;
    }
    else if (checKey == 'C')
    {
      i = 0;
      for (int j = 0; j < 4; j++)
      {
        password[i] = '\0';
        i++;
      }
      return false;
    }
  }


  if (i == 4)
  {
    delay(200);
    for (int j = 0; j < 4; j++)
    {
      initial_password[j] = EEPROM.read(x);
      x++;
    }
    initial_password[5] = '\0';


    if (!(strncmp(password, initial_password, 4)))
    {
      lcd.clear();
      lcd.print("PERMISSION GRANTED");
      delay(1500);

      i = 0;
      x = 0;
      return true;
    }

    else
    {
      lcd.clear();
      lcd.print("PERMISSION DENIED");
      delay(1500);
      i = 0;
      x = 0;
      return false;
    }
  }
}

//  function to change password
void change()

{
  int j = 0;
  int x = 100;
  bool cancel = false;
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

    if (key == 'C')
    {
      for (a = 0; a < 4; a++)
      {
        new_password[j++] = '\0'; // clear value
      }
      a = 0;
      lcd.clear();
      lcd.print("CANCELED");
      lcd.setCursor(0, 1);
      cancel = true;
      break;
    }

    else
    {
      cancel = false;
    }
    key = 0; // reset the key
  }

  delay(500);

  if ((strncmp(new_password, initial_password, 4)) && cancel != true) // compare the inputed password with the initially set password.
  {
    lcd.clear();
    lcd.print("WRONG Password"); //  this function enables one set password
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(1000);
  }

  else if (cancel == true)
  {
    return;
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
        EEPROM.update(x, key);
        j++;
        x++;
      }

      if (key == 'C')
      {
        for (a = 0; a < 4; a++)
        {
          initial_password[j++] = '\0'; // clear value
        }
        a = 0;
        lcd.clear();
        lcd.print("CANCELED");
        lcd.setCursor(0, 1);
        cancel = true;
        break;
      }
      key = 0; // reset the key
    }
    lcd.setCursor(0, 2);
    lcd.print("Pass Changed");
    delay(1000);
  }

  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  key_pressed = 0;
  cancel = false;
}


// this function sets initial password to "1234"
void initialpassword() {
  for (int j = 0; j < 4; j++)

    // default password is set to 49, which is the ASCII character for one
    EEPROM.update(j, j + 49); //increment from 1 to 4 and write to addresses 0 - 3

  for (int j = 0; j < 4; j++)
    initial_password[j] = EEPROM.read(j); // read the initial password values from EEPROM addresses 0 - 3
}



// function for the HOLD  button
void keypadEvent(KeypadEvent key_pressed) {
  switch (keypad_key.getState()) {

    case HOLD:
      if (key_pressed == '#' && voltLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET");
        delay(1000);
        voltLowerLimit = false;
        holdFlag = true;
        return;
        //        break;
      }

      if (key_pressed == '#' && voltUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET"); // assign the value from the voltmeter right here and write to EEPROM
        delay(1000);
        voltUpperLimit = false;
        holdFlag = true;
        return;
        //        break;
      }

      if (key_pressed == '#' && chargeUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("CHARGE UPPER LIMIT SET"); // set value from current sensor
        delay(1000);
        chargeUpperLimit = false;
        holdFlag = true;
        return;
        //        break;
      }

      if (key_pressed == '#' && currentCharge == true) {

        //use the current value
        lcd.clear();
        lcd.print("CURRENT CHARGE SET"); // set value from current sensor
        delay(1000);
        currentCharge = false;
        holdFlag = true;
        return;
        //        break;
      }

      if (key_pressed == '#' && fuelLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("FUEL LOWER LIMIT SET"); // set value from ultrasonic sensor
        delay(1000);
        fuelLowerLimit = false;
        holdFlag = true;
        return;
        //        break;
      }

      if (key_pressed == '#' && fuelUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("FUEL LOWER LIMIT SET"); //set value from ultrasonic sensor
        delay(1000);
        fuelUpperLimit = false;
        holdFlag = true;
        return;
        //        break;
      }

      else
      {
        holdFlag = false;
      }

    default:
      // Nothing to do in the other modes.
      holdFlag = false;
      return;
  }
}
