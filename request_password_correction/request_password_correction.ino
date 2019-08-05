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
char initial_password[4], new_password[4], lower_limit[4], upper_limit[4];


int i = 0;
int a = 0;
int e = 10; // start address for fuel lowerLimit
int f = 14; // start address for fuel upperLimit


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
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  //  initialpassword();
  keypad_key.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad_key.setHoldTime(5000); //set the amount of milliseconds the user will have to hold a button until the HOLD state is triggered. Default is 1000ms
}



void loop()
{

  digitalWrite(13, LOW);
  key_pressed = keypad_key.getKey();


  int x = 59;
  if (i == 4)
  {
    delay(200);
    for (int j = 0; j < 4; j++)
    {
      initial_password[j] = EEPROM.read(x);
      x++;
    }

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
      x = 0;
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
      x = 0;
    }
  }
}


//  function to change password
void change()

{
  int j = 0;
  int x = 59;
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
        EEPROM.write(x, key);
        j++;
        x++;
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
      if (key_pressed == '#' && voltLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET");
        delay(1000);
        voltLowerLimit = false;
        return;
      }

      if (key_pressed == '#' && voltUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("VOLT LOWER LIMIT SET"); // assign the value from the voltmeter right here and write to EEPROM
        delay(1000);
        voltUpperLimit = false;
        return;
      }

      if (key_pressed == '#' && chargeUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("CHARGE UPPER LIMIT SET"); // set value from current sensor
        delay(1000);
        chargeUpperLimit = false;
        return;
      }

      if (key_pressed == '#' && currentCharge == true) {

        //use the current value
        lcd.clear();
        lcd.print("CURRENT CHARGE SET"); // set value from current sensor
        delay(1000);
        currentCharge = false;
        return;
      }

      if (key_pressed == '#' && fuelLowerLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("FUEL LOWER LIMIT SET"); // set value from ultrasonic sensor
        delay(1000);
        fuelLowerLimit = false;
        return;
      }

      if (key_pressed == '#' && fuelUpperLimit == true) {

        //use the current value
        lcd.clear();
        lcd.print("FUEL LOWER LIMIT SET"); //set value from ultrasonic sensor
        delay(1000);
        fuelUpperLimit = false;
        return;
      }

      break;
  }
}

bool requestPassword()
{
  {
    int j;
    int x = 59;
    for (j = 0; j < 4; j++)
      initial_password[j] = EEPROM.read(x);
    lcd.clear();
    lcd.print("Password:");
    lcd.setCursor(11, 0);
    
    j = 0;
    while (j < 4)
    {
      char key = keypad_key.getKey();
      if (key) // if a key is pressed
      {
        new_password[j] = key; //tries to get the current 4 digit password the user is typing
        lcd.print(key);
        j++;

      }


      if (key == 'C')
      {
        for (j = 0; j < 5; j++)
        {
          new_password[j++] = "\0"; // clear value
        }
        lcd.clear();
        lcd.print("CANCELED");
        lcd.setCursor(0, 1);
        key = 0;
        return false;
        break;
      }
      key = 0; // reset the key
    }

    delay(1000); // change this back to 500

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("current password:");
    lcd.setCursor(0, 1);
    lcd.print(new_password);

    lcd.setCursor(0, 2);
    lcd.print("initial password:");
    lcd.setCursor(0, 3);
    lcd.print(initial_password);
    Serial.print("initial password is: " + String(initial_password) + "new password is: " + String(new_password));
    delay(10000);


    if (!(strncmp(new_password, initial_password, 4))) // compare the inputed password with the initially set password
    {
      lcd.clear();
      lcd.print("PERMISSION GRANTED"); //  this function enables one set password
      delay(1000);
      /*
         Set the new parameter here and write it to the EEPROM immediately
         remember the to leave enough gaps between the addresses
         Should this function be here or in the SWITCH-CASE statement????
         Let's test it first then decide later

         USE A WHILE LOOP TO ENTER PARAMETER
         HOW DO I ADD THE EVENT HANDLER TO THE WHILE LOOP???
      */
      return true;
    }

    else // if the new password is not the initial password
    {
      j = 0;
      lcd.clear();
      lcd.print("PERMISSION DENIED!");
      delay(1000);
      return false;

    }

    lcd.clear();
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    key_pressed = 0;
  }
}
