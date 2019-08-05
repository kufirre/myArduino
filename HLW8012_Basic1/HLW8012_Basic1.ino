#include <Arduino.h>
#include "HLW8012.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define SERIAL_BAUDRATE                 115200

// GPIOs
#define RELAY_PIN                       13
#define SEL_PIN                         5
#define CF1_PIN                         11
#define CF_PIN                          12

// Check values every 2 seconds
#define UPDATE_TIME                     1000

int voltage = 0;

// Set SEL_PIN to HIGH to sample current
// This is the case for Itead's Sonoff POW, where a
// the SEL_PIN drives a transistor that pulls down
// the SEL pin in the HLW8012 when closed
#define CURRENT_MODE                    HIGH

// These are the nominal values for the resistors in the circuit
#define CURRENT_RESISTOR                0.001
#define VOLTAGE_RESISTOR_UPSTREAM       ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM     ( 1000 ) // Real 1.009k

HLW8012 hlw8012;

void unblockingDelay(unsigned long mseconds) {
  unsigned long timeout = millis();
  while ((millis() - timeout) < mseconds) delay(1);
}

void calibrate() {

  // Let's first read power, current and voltage
  // with an interval in between to allow the signal to stabilise:

  hlw8012.getActivePower();

  hlw8012.setMode(MODE_CURRENT);
  unblockingDelay(2000);
  hlw8012.getCurrent();

  hlw8012.setMode(MODE_VOLTAGE);
  unblockingDelay(2000);
  hlw8012.getVoltage();

  // Calibrate using a 60W bulb (pure resistive) on a 230V line
  hlw8012.expectedActivePower(60.0);
  hlw8012.expectedVoltage(227.0);
  hlw8012.expectedCurrent(60.0 / 227.0);

  // Show corrected factors
  Serial.print("[HLW] New current multiplier : "); Serial.println(hlw8012.getCurrentMultiplier());
  Serial.print("[HLW] New voltage multiplier : "); Serial.println(hlw8012.getVoltageMultiplier());
  Serial.print("[HLW] New power multiplier   : "); Serial.println(hlw8012.getPowerMultiplier());
  Serial.println();

}

void setup() {

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();

  // initialize the LCD
  lcd.begin();
  lcd.clear();
  // Turn on the blacklight and print a message.
  lcd.backlight();

  // Close the relay to switch on the load
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  // Initialize HLW8012
  // void begin(unsigned char cf_pin, unsigned char cf1_pin, unsigned char sel_pin, unsigned char currentWhen = HIGH, bool use_interrupts = false, unsigned long pulse_timeout = PULSE_TIMEOUT);
  // * cf_pin, cf1_pin and sel_pin are GPIOs to the HLW8012 IC
  // * currentWhen is the value in sel_pin to select current sampling
  // * set use_interrupts to false, we will have to call handle() in the main loop to do the sampling
  // * set pulse_timeout to 500ms for a fast response but losing precision (that's ~24W precision :( )
  hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, CURRENT_MODE, false, 500000);

  // These values are used to calculate current, voltage and power factors as per datasheet formula
  // These are the nominal values for the Sonoff POW resistors:
  // * The CURRENT_RESISTOR is the 1milliOhm copper-manganese resistor in series with the main line
  // * The VOLTAGE_RESISTOR_UPSTREAM are the 5 470kOhm resistors in the voltage divider that feeds the V2P pin in the HLW8012
  // * The VOLTAGE_RESISTOR_DOWNSTREAM is the 1kOhm resistor in the voltage divider that feeds the V2P pin in the HLW8012
  hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);

  // Show default (as per datasheet) multipliers
  Serial.print("[HLW] Default current multiplier : "); Serial.println(hlw8012.getCurrentMultiplier());
  Serial.print("[HLW] Default voltage multiplier : "); Serial.println(hlw8012.getVoltageMultiplier());
  Serial.print("[HLW] Default power multiplier   : "); Serial.println(hlw8012.getPowerMultiplier());
  Serial.println();

//      calibrate();

}

void loop() {

  static unsigned long last = millis();

  // This UPDATE_TIME should be at least twice the minimum time for the current or voltage
  // signals to stabilize. Experimentally that's about 1 second.
  if ((millis() - last) > UPDATE_TIME) {

    voltage = hlw8012.getVoltage();
    last = millis();
    Serial.print("[HLW] Active Power (W)    : "); Serial.println(hlw8012.getActivePower());
    Serial.print("[HLW] Voltage (V)         : "); Serial.println(voltage);
    Serial.print("[HLW] Current (A)         : "); Serial.println(hlw8012.getCurrent());
    Serial.print("[HLW] Apparent Power (VA) : "); Serial.println(hlw8012.getApparentPower());
    Serial.print("[HLW] Power Factor (%)    : "); Serial.println((int) (100 * hlw8012.getPowerFactor()));
    Serial.println();


// print on 20x4 LCD display
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("POWER METER");
    lcd.setCursor(0, 1);
    lcd.print("Voltage(V) = ");
    lcd.setCursor(13, 1);
    lcd.print(voltage);
    lcd.setCursor(0, 2);
    lcd.print("Current(A) = ");
    lcd.setCursor(13, 2);
    lcd.print(hlw8012.getCurrent());
    lcd.setCursor(0, 3);
    lcd.print("Watts(W)   = ");
    lcd.setCursor(13, 3);
    lcd.print(hlw8012.getActivePower());


    // When not using interrupts we have to manually switch to current or voltage monitor
    // This means that every time we get into the conditional we only update one of them
    // while the other will return the cached value.
    hlw8012.toggleMode();


  }

}
