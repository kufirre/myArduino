#include "EmonLib.h"             // Include Emon Library

#define VOLT_CAL 462.3  // formerly 466.7
#define CURRENT_CAL 15.152

EnergyMonitor emon1;             // Create an instance

void setup()
{
  Serial.begin(9600);

  emon1.voltage(A0, VOLT_CAL, 1.4);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A1, CURRENT_CAL);       // Current: input pin, calibration.
}

void loop()
{
//  Serial.println("gotten here");
  emon1.calcVI(20, 2000);                         // Calculate all. No.of half wavelengths (crossings), time-out
//  Serial.println("passed VI test");
  emon1.calcF(50, 2000);     // 50 zero crossings
//  Serial.println("passed frequency test");

  double currentDraw     = emon1.Irms;             //extract Irms into Variable
  int supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  double freq = emon1.frequency;
  double RP = emon1.realPower;
  double AP = emon1.apparentPower;
  double PF = emon1.powerFactor;

  Serial.print("Frequency: ");
  Serial.println(String(freq) + "Hz");

  Serial.print("Voltage: ");
  Serial.println(String(supplyVoltage) + "V");

  Serial.print("Current: ");
  Serial.println(String(currentDraw) + "A");

  Serial.print("Real Power: ");
  Serial.println(String(RP) + "KVA");

  Serial.print("Apparent Power: ");
  Serial.println(String(AP) + "Watts");

  Serial.print("Power Factor: ");
  Serial.println(String(PF));    
  Serial.println("\n\n");

}
