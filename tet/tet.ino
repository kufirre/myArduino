void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
}

void loop() {
 
Serial.println(fun(2));

}

int fun(int n)
 {
  if (n==4)
  {
    return 2;
  }
  else
  return 2 * fun (n+1);
 }
