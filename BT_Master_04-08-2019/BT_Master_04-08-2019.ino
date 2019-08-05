//char string[32];
char string;
String byteRead = "";
double height = 0.00, ultrasonicReading = 0.00;

unsigned long heightMillis = 0;
unsigned long heightInterval = 15000;
//float ;


void setup()
{
  Serial.begin(9600);
  Serial.println("MASTER(HC-05):");

  // HC-06 default baud rate is 9600
  Serial1.begin(9600);
}

void loop()
{
  byteRead = "";
  while (Serial1.available() > 0) {
    string = Serial1.read();
    byteRead += string;
  }
  height = byteRead.toDouble() / 100.0d;


  if (height != 0.00) {
    ultrasonicReading = height;
  }
  if (height == 0.00) {
    if ((millis() - heightMillis) >= heightInterval) {
      ultrasonicReading = 0.00;
      heightMillis = millis();
    }
  }


  // test for vertical cylinder: circumference = 1.345, emptyHeight = 3.77
  Serial.println("height is " + String(height) + " and quantity of fuel is " + String(verticalCylinder(ultrasonicReading, 3.77, 1.345)) + " litres" + '\n');

}



/*
   function to get the volume of fuel in a horizontal cylinder
*/

double horizontalCylinder(float F, float emptyHeight, float c, float L) {
  double a, r, m, f, f1, teta, a1, m1, teta1, vTank, vSegment, fuelCapacity;

  // getting the radius from the circumference
  r = c / (2 * M_PI);

  if (emptyHeight > F) {
    f = emptyHeight - F;
  }

  if (F == 0.00) {
    f = 0.00;
  }
  else {
    f = 0.00;
  }

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

double verticalCylinder(float F, float emptyHeight, float c) {
  double r, h, fuelCapacity;

  r = c / (2 * M_PI);

  if (emptyHeight > F) {
    h = emptyHeight - F;
  }

  if (F == 0.00) {
    h = 0.00;
  }
  //  else {
  //    h = 0.00;
  //  }

  fuelCapacity = (M_PI * r * r * h) * 1000;

  return fuelCapacity;
}


/*
   function to determine the volume of fuel in a rectanggular tank
*/

double rectangleTank(float F, float emptyHeight, float b, float L) {
  double fuelCapacity, h;

  if (emptyHeight > F) {
    h = emptyHeight - F;
  }
  if (F == 0.00) {
    h = 0.00;
  }
  else {
    h = 0.00;
  }

  //  h = emptyHeight - F;
  fuelCapacity = h * b * L;

  return fuelCapacity;
}


/*
   function to get the volume of fuel in a horizontal oval tank
*/
double horizontalOval(float F, float emptyHeight, float a, float w, float L) {
  double f, vTank, vFillRect, vFillCyl, vCylTank, fuelCapacity;
  float A, r, m, h, teta;
  float f1, A1, m1, teta1, vSegment;

  h = emptyHeight;
  r = h / 2;
  if (h > F) {
    f = h - F;
  }
  if (F == 0.00) {
    f = 0.00;
  }
  else {
    f = 0.00;
  }
  //  f = h - F;

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

double verticalOval(float F, float emptyHeight, float a, float w, float L) {
  double vTank, vFillRect, vRect, vFillCyl, vCylTank, fuelCapacity;
  float A, r, m, h, teta;
  float f, f1, A1, m1, teta1, vSegment;


  h =  emptyHeight;
  r = w / 2;

  if (h > F) {
    f = h - F;
  }
  if (F == 0.00) {
    f = 0.00;
  }
  else {
    f = 0;
  }

  //  f = h - F;
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
