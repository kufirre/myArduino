#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3); // RX | TX

//ultrasonic sensor parameters
const int trigPin = 7;
const int echoPin = 6;
float dist = 0;

unsigned long prevMillis = 0;
unsigned long currentMillis = 0;

//variables needed to store values
int arraysize = 9;  //quantity of values to find the median (sample size). Needs to be an odd number

//declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
int rangevalue[] = {  0, 0, 0, 0, 0, 0, 0, 0, 0};

int modE;

void setup()
{
  //initialize serial monitor
  Serial.begin(9600);

  // HC-06 default baud rate is 9600
  BTSerial.begin(9600);

  //initialize Ultrasonic parameters
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(500);
  Serial.println("ready!");
}


void loop() {
  //  Serial.println("here first");

  if (millis() - currentMillis >= 1000l) {
    //    Serial.println("here next");
    for (int i = 0; i < arraysize; i++)
    {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(12);
      digitalWrite(trigPin, LOW);
      dist = pulseIn(echoPin, HIGH);

      //      Serial.println(dist);

      if ((millis() - prevMillis) >= 10) {
        rangevalue[i] = dist / 58;
        prevMillis = millis();
      }
    }

    isort(rangevalue, arraysize);

    modE = bestSample(rangevalue, arraysize);
    //    modE = modE / 100; //convert from cm to m


    // send data through the HC-06 to the HC05 connected to the other Arduino
    BTSerial.print(modE);

    Serial.println(modE);
    currentMillis = millis();

  }

}




//Function to print the arrays.

void printArray(int *a, int n)

{
  for (int i = 0; i < n; i++)
  {
    //    Serial.print(a[i], DEC);
    //    Serial.print(' ');
  }
  //  Serial.println();
}

//Sorting function
// sort function (Author: Bill Gentles, Nov. 12, 2010)

void isort(int *a, int n) {

  //  *a is an array pointer function
  for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}


uint16_t bestSample(uint16_t *x, uint8_t n) // x is the rangevalue, gotten from the while loop while n is the array size
{
  bool bimodal = false; // set true if samples are bi or multimodal
  uint16_t selectedSample = 0;

  uint8_t maxCount  = 0;              // longest sequence found so far - 1

  // Search the samples for the longest sequence.

  for (uint8_t i = 0; i < (n - 1); i++)
  {
    // Find a sequence of identical values if there is one.

    uint8_t count = 0;              // number of identical samples - 1

    while (x[i] == x[i + 1] && i < (n - 1))
    {
      count++;
      i++;
    }

    // Keep track if this is the longest longest sequence so far.
    //
    if (count > maxCount)
    {
      selectedSample = x[i];
      maxCount = count;
      bimodal  = false;       // longest, so it's unique
    }
    //
    // If the sample set has multiple identical-length sequences, it's bi or multimodal.
    //
    else if (count != 0 && count == maxCount)
    {
      bimodal = true;
    }
  }

  // Return the median if there is no sequence or no unique sequence.

  if ( selectedSample == 0 || bimodal )
  {
    selectedSample = x[(n / 2)];
  }

  return selectedSample;
}
