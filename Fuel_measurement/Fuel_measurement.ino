
#include <SimpleKalmanFilter.h>

const int trigPin = A0;
const int echoPin = 6;
//float estimated_value = 0;
float dist = 0;
float smoothedValue;


SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(obstacle(0.8));
  Serial.print(","); 
  Serial.println(obstacle(0.0));
  
  delay(200);
}

float obstacle(float filterValue)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(trigPin, LOW);
  dist = pulseIn(echoPin, HIGH);
  delayMicroseconds(10000);
  dist = dist / 58;

  //implementation of a simple low-pass filter
  smoothedValue = (dist * (1 - filterValue)) + (smoothedValue  *  filterValue);
  return (int)smoothedValue;


  //come up with a filtering, smoothening algorithm here for ultrasonic
  //  estimated_value = simpleKalmanFilter.updateEstimate(dist);
  //  return estimated_value;
}

// Simple low pass filter. filterValue determines smoothness. 0 = off; 0.9999 = max
