#include <math.h>

const int sensorPin = A0;
long sampleTime = 0;
int sampleCount = 0;
float sampleValue;
long sampleSqrSum;
double sampleMeanSqr;
double sampleRMS;
double x, y;
double vpc;
double iRMS;

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  //  result = 1125300L / result; // Back-calculate AVcc in mV
  result = 1111040L / result;
  return result;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() > sampleTime + 1) {
    sampleValue = (analogRead(sensorPin));// - 512);
    sampleSqrSum += sq(sampleValue);
    sampleCount++;
  }


  if (sampleCount == 1000) {
    sampleMeanSqr = (sampleSqrSum / sampleCount);
    sampleRMS = sqrt(sampleMeanSqr);
    x =  sampleRMS;
//    y = (8 * pow(10, -12) * pow(x, 5)) - (pow(10, -8) * pow(x, 4)) + (9 * pow(10, -6) * pow(x, 3)) - (0.0031 * pow(x, 2)) + (0.6609 * x) + (4.4992); //y = 8E-12x5 - 1E-08x4 + 9E-06x3 - 0.0031x2 + 0.6609x + 4.4992
//    y = (9*pow(10,-07)*pow(x,3)) - (0.0011*pow(x,2)) + (0.4595*x) + 8.4253;//y = 9E-07x3 - 0.0011x2 + 0.4595x + 8.4253
//    y = (-3*pow(10,-12)*pow(x,5)) + (6*pow(10,-9)*pow(x,4)) - (3*pow(10,-6)*pow(x,3)) - (0.0002*pow(x,2)) + (0.4153*x) + 7.2644;  //y = -3E-12x5 + 6E-09x4 - 3E-06x3 - 0.0002x2 + 0.4153x + 7.2644
//    y = 0.0002*pow(x,2) + 0.0379*x + 36.316; //y = 0.0002x2 + 0.0379x + 36.316
//      y = (-4*pow(10,-14)*pow(x,6)) + (pow(10,-10)*pow(x,5)) - (pow(10,-7)*pow(x,4)) + (4*pow(10,-5)*pow(x,3)) - (0.0086*pow(x,2)) + (0.9887*x) + 2.3089;      //y = -4E-14x6 + 1E-10x5 - 1E-07x4 + 4E-05x3 - 0.0086x2 + 0.9887x + 2.3089




    if (sampleRMS == 0.00){
      y = 0;
    }
    Serial.println(sampleRMS);
    Serial.println("The Voltage is " +String(long(y)) +"Volts");
    sampleSqrSum = 0;
    sampleCount = 0;
    delay(500);
  }


}
