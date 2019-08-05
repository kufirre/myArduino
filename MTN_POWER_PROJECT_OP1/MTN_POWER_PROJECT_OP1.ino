/*
 * you should probably determine the soc using both current integration and open circuit voltage method.
 * It is possible to cover the weakness of both techniques and provide an accurate monitoring technique.
 * I can provide you the codes if you need
 */


#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int mVperAmp = 100; const float chargingdfficiency = 0.7;
int RawValue = 0, ACSoffset = 2500;
double Voltage = 0, Amps = 0;
double g, q;
int m = 0, c = 0, d;
void resett() {
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
}

void setup() {
  pinMode(A0, INPUT); pinMode(A1, INPUT);
  pinMode(7, OUTPUT); pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  delay(10); lcd.begin(20, 4); delay(10);
}

void loop() {
  resett();
  double vini = analogRead(1); vini = vini * 5 / 1023;
  vini = 5 * vini; Serial.print(vini, 3); vini = vini / 6;
  double soc = ((666.6667) * vini) - 1300; delay(10);
  q = soc * .42;
  if (soc < 30) {
    m = 1;  //charging
    resett();
    digitalWrite(7, HIGH);
  }
  if (soc >= 100) {
    m = 0;  // DISCHARGING
    resett();
    digitalWrite(8, HIGH);
  }
  if (m == 0) {
    resett();
    digitalWrite(8, HIGH);
  }
  delay(10);
  while (1) {
    RawValue = analogRead(0);
    Voltage = (RawValue / 1024.0) * 5000;
    Amps = ((Voltage - ACSoffset) / mVperAmp); Amps = -Amps;
    if ((Amps < .098) && (Amps > -.098)) {
      Amps = 0;
    }
    if (Amps > 9) {
      resett();
      lcd.clear();
      lcd.display();
      lcd.setCursor(1, 0);
      lcd.print("Overload"); digitalWrite(9, HIGH);
      while (1);
    }
    if (m == 0) {
      if (Amps <= 1.5) {
        g = 1.183;
      }
      else if (Amps <= 2) {
        g = 0.042 * Amps + 1.12;
      }
      else if (Amps <= 4) {
        g = 0.028 * Amps + 1.148;
      }
      else if (Amps <= 5) {
        g = 0.032 * Amps + 1.132;
      }
      else if (Amps <= 6) {
        g = 0.03 * Amps + 1.142;
      }
      else if (Amps <= 7) {
        g = 0.325 * Amps - 0.628;
      }
      else if (Amps <= 7.5) {
        g = 0.066 * Amps + 1.185;
      }
      else if (Amps <= 8) {
        g = 0.028 * Amps + 1.47;
      }
      else if (Amps <= 10) {
        g = 1.694;
      }
      soc = (q - Amps * g * 0.0002778) * 2.38095; q = soc * .42;
      if (soc < 5) {
        m = 1;
        resett();
        digitalWrite(7, HIGH);
      }
    }
    if (m == 1) {
      soc = (q - Amps * chargingdfficiency * 0.0002778) * 2.38095;
      q = soc * .42;
      if (soc >= 100) {
        m = 0;
        resett();
        digitalWrite(8, HIGH);
      }
    }
    lcd.clear();
    lcd.display();
    lcd.setCursor(1, 0);
    lcd.print("Soc = ");
    lcd.setCursor(13, 0);
    lcd.print(soc, 1);
    lcd.setCursor(19, 0);
    lcd.print("%");
    lcd.setCursor(1, 1);
    lcd.print("Amps= ");
    lcd.setCursor(13, 1);
    lcd.print(Amps, 1);
    if (m == 1) {
      lcd.setCursor(1, 2);
      lcd.print("Charging");
    }
    else {
      lcd.setCursor(1, 2);
      lcd.print("Discharge");
    }
    lcd.setCursor(1, 3); lcd.print("Normal");
    Serial.println((vini), 3);
    Serial.print("Amps = ");
    Serial.println(Amps, 9);
    Serial.print(" Soc = ");
    Serial.println(soc, 9);

    for (d = 0; ((Amps == 0) && (d <= 1200)); d++) {
      RawValue = analogRead(0);
      Voltage = (RawValue / 1024.0) * 5000;
      Amps = ((Voltage - ACSoffset) / mVperAmp); Amps = -Amps;
      if (d == 1200) {
        vini = analogRead(1); vini = vini * 25 / (6 * 1023);
        soc = ((666.6667) * vini) - 1300; d = 0;
      }
    }

    delay(1000);
  }
}
