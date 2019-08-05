#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int light1  = 10;
int buzzerPin = 11;
bool pinState;

void setup() {
  Serial.begin(9600);
  pinMode(light1, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinState = digitalRead(light1);
  //while (!Serial) ; // wait for serial
  //delay(200);
  //Serial.println("DS1307RTC Read Test");
  //Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
    if ((tm.Hour >= 8 && tm.Minute >= 0 && tm.Minute < 5))
    {
      digitalWrite(light1, HIGH);
      delay(100);
    }
    else {
      digitalWrite(light1, LOW);
    }

  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(3000);
  }
  pinState = digitalRead(light1);
  if (pinState == HIGH){
   digitalWrite(buzzerPin, HIGH);
   delay(200);
   digitalWrite(buzzerPin, LOW);
   delay(700); 
  }
  else{
    digitalWrite(buzzerPin, LOW);
    digitalWrite(light1, LOW);
  }
}
