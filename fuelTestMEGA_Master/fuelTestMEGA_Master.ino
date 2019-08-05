/*
   These are the functions for calculating Peukert's Constant and determining the fuel capacity based on the ultrasonic sensor readings
   ALL INFORMATION SHOULD BE REPRESENTED IN S.I. Units (meters, litres, etc.)
*/

#include <math.h>

double dist = 0.00;

void setup() {
  Serial.begin(9600);
  Serial.println("MASTER(HC-05):");

  // HC-06 default baud rate is 9600
  Serial1.begin(9600);

}

void loop() {
  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (Serial1.available())
    Serial.write(Serial1.read());

  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
    Serial1.write(Serial.read());


}


/*
   function to get the volume of fuel in a horizontal cylinder
*/

double horizontalCylinder(float F, float c, float L) {
  double a, r, m, f, f1, teta, a1, m1, teta1, vTank, vSegment, fuelCapacity;

  // getting the radius from the circumference
  r = c / (2 * M_PI);
  f = 2 * r - F;
  m = r - f;

  //volume of tank
  vTank = M_PI * r * r * L;

  teta = 2 * acos(m / r);
  a = 0.5 * r * r * (teta * sin(teta));


  //if the fuel level is less than the radius of the tank
  if (f < r) {
    fuelCapacity = (a * L) * 1000;    // convert from m3 to litres
  }

  //if the fuel level is greater than the radius of the tank
  if (f > r) {
    f1 = (2 * r) - f;
    m1 = r - f1;

    teta1 = 2 * acos(m1 / r);
    a1 = 0.5 * r * r * (teta1 * sin(teta1));

    vSegment = a1 * L;

    fuelCapacity = (vTank - vSegment) * 1000;   // convert from m3 to litres

  }

  return fuelCapacity;
}

/*
    function to get the volume of fuel in a vertical cylinder
*/

double verticalCylinder(float h, float c, float L) {
  double r, fuelCapacity;

  r = c / (2 * M_PI);
  fuelCapacity = (M_PI * r * r * h) * 1000;

  return fuelCapacity;
}


/*
   function to determine the volume of fuel in a rectanggular tank
*/

double rectangleTank(float h, float b, float L) {
  double fuelCapacity;
  fuelCapacity = h * b * L;

  return fuelCapacity;
}


/*
   function to get the volume of fuel in a horizontal oval tank
*/
double horizontalOval(float F, float a, float w, float h, float L) {
  double f, vTank, vFillRect, vFillCyl, vCylTank, fuelCapacity;
  float A, r, m, teta;
  float f1, A1, m1, teta1, vSegment;

  r = h / 2;
  f = h - F;

  vTank = ((M_PI * r * r) + (2 * r * a)) * L;
  vCylTank = M_PI * r * r * L;
  vFillRect = f * a * L;

  if (f  < r)
  {
    m = r - f;
    teta = 2 * acos(m / r);
    A = 0.5 * r * r * (teta * sin(teta));
    vFillCyl = A * L;
    fuelCapacity = (vFillRect + vFillCyl) * 1000;
  }

  if (f > r) {

    f1 = (2 * r) - h;
    m1 = r - f1;
    teta1 = 2 * acos(m1 / r);
    A1 = 0.5 * r * r * (teta1 * sin(teta1));

    vSegment = A1 * L;

    vFillCyl = vCylTank - vSegment;
    fuelCapacity = (vFillRect + vFillCyl) * 1000;
  }

  return fuelCapacity;
}


/*
   function to get the volume of fuel in vertical oval cylinder
*/

double verticalOval(float F, float a, float w, float h, float L) {
  double vTank, vFillRect, vRect, vFillCyl, vCylTank, fuelCapacity;
  float A, r, m, teta;
  float f, f1, A1, m1, teta1, vSegment;

  r = w / 2;
  f = h - F;
  vCylTank = M_PI * r * r * L;
  vFillRect = w * a * L;
  vTank = ((M_PI * r * r) + (2 * r * a)) * L;

  if (f < r) {
    m = r - f;
    teta = 2 * acos(m / r);
    A = 0.5 * r * r * (teta * sin(teta));

    vFillCyl = A * L;
    fuelCapacity = vFillCyl * 1000;
  }

  if ((f > r) && (f < (r + a))) {
    vFillCyl = (vCylTank / 2);
    vRect = w * (f - r) * L;

    fuelCapacity = (vFillCyl + vRect) * 1000;
  }

  if ((f > (a + r)) && (f < h)) {
    f1 = h - f;
    m1 = r - f1;
    teta1 = 2 * acos(m1 / r);
    A1 = 0.5 * r * r * (teta1 * sin(teta1));

    vSegment = A1 * L;

    fuelCapacity = (vTank - vSegment) * 1000;

  }

  return fuelCapacity;

}
