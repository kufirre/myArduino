float distance;
int buzzer;


void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  Serial.println(distance);
  distance = checkdistance();
  Serial.println(String(distance) + "cm");
  //  delay(100);
  if (distance <= 20)
  {
    digitalWrite(2, HIGH);
  }
  else
  {
    digitalWrite(2, LOW);
  }
}

  float checkdistance() {
    digitalWrite(A0, LOW);
    delayMicroseconds(2);
    digitalWrite(A0, HIGH);
    delayMicroseconds(10);
    digitalWrite(A0, LOW);
    float distance = pulseIn(7, HIGH) / 58.00;
    delay(10);
    return distance;
  }
