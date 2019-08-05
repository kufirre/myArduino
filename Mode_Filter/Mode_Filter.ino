#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 parameters
RF24 radio(9, 10); // CE, CSN
const uint64_t pipes[2] = {
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL
};

//ultrasonic sensor parameters
const int trigPin = A0;
const int echoPin = 6;
float dist = 0;

//variables needed to store values
int arraysize = 9;  //quantity of values to find the median (sample size). Needs to be an odd number

//declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
int rangevalue[] = {  0, 0, 0, 0, 0, 0, 0, 0, 0};

int modE;

void setup()
{
  //initialize serial monitor
  Serial.begin(115200);

  //initialize radio parameters
  radio.begin();
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();

  //initialize Ultrasonic parameters
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(500);
}


void loop() {

  for (int i = 0; i < arraysize; i++)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(12);
    digitalWrite(trigPin, LOW);
    dist = pulseIn(echoPin, HIGH);
    delayMicroseconds(10000);
    rangevalue[i] = dist / 58;
    delay(10);
  }

  //  Serial.print("Unsorted: ");
  printArray(rangevalue, arraysize);
  isort(rangevalue, arraysize);
  //  Serial.print("Sorted: ");
  printArray(rangevalue, arraysize);

  modE = bestSample(rangevalue, arraysize);


  // radio stuff
  radio.stopListening();
  bool ok = radio.write( &modE, sizeof(modE) );

  //set radio to listening mode. Not relevant for this application
  //  radio.startListening();
  //
  //  unsigned long started_waiting_at = millis();
  //  bool timeout = false;
  //  while ( ! radio.available() && ! timeout )
  //    if (millis() - started_waiting_at > 250 )
  //      timeout = true;
  //
  //  if ( timeout )
  //  {
  //    Serial.println("Failed, response timed out.");
  //    digitalWrite(led, HIGH);
  //  }
  //  else
  //  {
  //    radio.read( &response, sizeof(response) );
  //    digitalWrite(led, LOW);
  //  }
  // end of radio stuff

  //  Serial.print("The mode/median is: ");
  Serial.print(dist / 58);
  Serial.print(",");
  Serial.println(modE);
  //  Serial.println();
  delay(1000);

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
