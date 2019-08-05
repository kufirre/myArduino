#include <SimpleKalmanFilter.h>
//#include <NeoSWSerial.h>

//NeoSWSerial soft(9, 10);

int leftLine = A1;
int middleLine = A2;
int rightLine = A3;

int MA_dir = 4;
int MB_dir = 7;
int MA_spd = 5;
int MB_spd = 6;

bool manMode = false;
bool lineMode = false;
bool autoMode = false;
bool followMode = false;

bool StopMode = false;
bool frontMode = false;
bool backMode = false;
bool leftMode = false;
bool rightMode = false;

int buzzerPin = 13; //meant for obstacle alert

char val = ' ';
char val1 = ' ';

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

float dist = 0;
float estimated_value = 0;


void setup() {
  pinMode(leftLine, INPUT);
  pinMode(middleLine, INPUT);
  pinMode(rightLine, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(2, INPUT);
  pinMode(MA_dir, OUTPUT);
  pinMode(MB_dir, OUTPUT);
  pinMode(MA_spd, OUTPUT);
  pinMode(MB_spd, OUTPUT);
  Serial.begin(9600);
  //  soft.begin(9600);
}

void loop()
{

  while (Serial.available() > 0) {
    val = Serial.read();
    switch (val) {

      case 'M':
        manMode = true;
        lineMode = false;
        autoMode = false;
        followMode = false;
        break;

      case 'f':
        manMode = false;
        lineMode = false;
        autoMode = false;
        followMode = true;
        break;

      case 'T':
        manMode = false;
        lineMode = true;
        autoMode = false;
        followMode = false;
        break;

      case 'A':
        manMode = false;
        lineMode = false;
        autoMode = true;
        followMode = false;
        break;

      default:
        manMode = true;
        lineMode = false;
        autoMode = false;
        followMode = false;
    }
  }

  if (manMode) {
    while (Serial.available() > 0) {
      val1 = Serial.read();

      switch (val1) {
        case 'S':
          StopMode = true;
          frontMode = false;
          backMode = false;
          leftMode = false;
          rightMode = false;
          break;


        case 'F':
          StopMode = false;
          frontMode = true;
          backMode = false;
          leftMode = false;
          rightMode = false;
          break;

        case 'B':
          StopMode = false;
          frontMode = false;
          backMode = true;
          leftMode = false;
          rightMode = false;
          break;

        case 'L':
          StopMode = false;
          frontMode = false;
          backMode = false;
          leftMode = true;
          rightMode = false;
          break;

        case 'R':
          StopMode = false;
          frontMode = false;
          backMode = false;
          leftMode = false;
          rightMode = true;
          break;

        default:
          StopMode = true;
          frontMode = false;
          backMode = false;
          leftMode = false;
          rightMode = false;
      }
    }

    if (StopMode) {
      Stop();
    }

    if (frontMode) {
      front();
    }

    if (backMode) {
      back();
    }

    if (leftMode) {
      left();
    }

    if (rightMode) {
      right();
    }
  }

  if (lineMode) {
    trackLine();
  }

  if (autoMode) {
    autoDrive();
  }

  if (followMode) {
    follower();
  }

}


void front()
{
  digitalWrite(MA_dir, HIGH);
  digitalWrite(MB_dir, HIGH);
  analogWrite(MA_spd, 80);
  analogWrite(MB_spd, 80);
}

void back()
{
  digitalWrite(MA_dir, LOW);
  digitalWrite(MB_dir, LOW);
  analogWrite(MA_spd, 80);
  analogWrite(MB_spd, 80);
}

void Stop()
{
  digitalWrite(MA_dir, HIGH);
  digitalWrite(MB_dir, HIGH);
  analogWrite(MA_spd, 0);
  analogWrite(MB_spd, 0);
}

void left()
{
  digitalWrite(MA_dir, LOW);
  digitalWrite(MB_dir, HIGH);
  analogWrite(MA_spd, 80);
  analogWrite(MB_spd, 80);
}

void right()
{
  digitalWrite(MA_dir, HIGH);
  digitalWrite(MB_dir, LOW);
  analogWrite(MA_spd, 80);
  analogWrite(MB_spd, 80);
}

void trackLine()
{
  if (digitalRead(middleLine))
  {
    if (digitalRead(rightLine) && !(digitalRead(leftLine)))
    {
      front();
    }
    else if (!(digitalRead(rightLine)) && digitalRead(leftLine))
    {
      front();
    }
    else
    {
      front();
    }
  }
  else
  {
    if (digitalRead(rightLine) && !(digitalRead(leftLine)))
    {
      left();
    }
    else if (!(digitalRead(rightLine)) && digitalRead(leftLine))
    {
      right();
    }
    else
    {
      Stop();
    }
  }
}

float obstacle()
{
  digitalWrite(A0, LOW);
  delayMicroseconds(2);
  digitalWrite(A0, HIGH);
  delayMicroseconds(12);
  digitalWrite(A0, LOW);
  dist = pulseIn(2, HIGH);
  delayMicroseconds(10000);
  dist = dist / 58;
  //come up with a filtering, smoothening algorithm here for ultrasonic
  estimated_value = simpleKalmanFilter.updateEstimate(dist);
  return estimated_value;
}

void autoDrive()
{
  float dist1;
  float dist2;
  float dist3;

  dist1 = obstacle();
  if (dist1 < 35)
  {
    buzzer();
    Stop();
    delay(200);
    left();
    delay(300);
    Stop();
    dist2 = obstacle();
    delay(400);
    right();
    delay(600);
    Stop();
    dist3 = obstacle();
    delayMicroseconds(400);
    if (dist2 > dist3)
    {
      left();
      delay(600);
      front();
    }
    else
    {
      front();
    }
  }
  else
  {
    front();
  }

  Serial.println(dist1);
}

void buzzer()
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(1000);
  }
  delayMicroseconds(50000);
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(1000);
  }
}

void follower()
{
  dist = obstacle();

  //come up with a filtering, smoothening algorithm here for ultrasonic
  estimated_value = simpleKalmanFilter.updateEstimate(dist);

  if (estimated_value < 55 && estimated_value > 30) {
    //This condition is to move forward the robot when the object is in the range of 14 to 25 centimeters.
    front();
  }

  if (estimated_value < 30 && estimated_value > 25) {
    //This condition is to make the robot stable when the object is in the range of 12 to 14 centimeters.
    Stop();
  }

  if (estimated_value < 25 && estimated_value > 2) {
    //This condition is to move backward the robot when the object is in the range of 2 to 12 centimeters.
    back();
  }

  if (estimated_value > 40) {
    //This condition is to stop the robot when the object is in the out of range i.e greater than 25 centimeters.
    Stop();
  }
}
