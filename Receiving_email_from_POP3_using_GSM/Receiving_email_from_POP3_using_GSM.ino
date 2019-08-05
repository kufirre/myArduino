/*  
 *  3G + GPS shield
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Alejandro Gallego & Victor Boria
 */

//Write here you server and account data
const char server[ ] = "*********";      
const char user_name[ ] = "*********";   
const char password[ ] = "*********";    
const char port[ ] = "***";     

//Write here you SIM card data
const char sim_pin[] = "****";
const char sim_apn[] = "*********";
const char sim_user[] = "*********";
const char sim_password[] = "*********";

char data[512];
char path[50];
int8_t answer;
int onModulePin= 2;
int counter;
unsigned long previous;
char aux_str[128];

void setup(){

  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);   

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", sim_pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets pop3 server
  sprintf(data, "AT+POP3SRV=\"%s\",\"%s\",\"%s\",%s", server, user_name, password, port);
  sendATcommand(data, "OK", 2000);

  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", sim_apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", sim_user, sim_password);
  sendATcommand(aux_str, "OK", 2000);

  delay(2000);

  // logs into the server
  answer = sendATcommand("AT+POP3IN", "OK",10000);
  if (answer == 1)
  {
    Serial.println("Logged into the server");

    // gets the first email
    answer = sendATcommand("AT+POP3GET=1", "C:/Email/",10000);
    if (answer == 1)
    {
      counter = 0;
      while(Serial.available()==0);
      do{
        path[counter] = Serial.read();
        counter++;
        if ((path[counter-1] == ' ') || (path[counter-1] == ','))
        {
          counter--;    
        }
        while(Serial.available()==0);
      }
      while (path[counter-1] != 0x0D);

      path[counter-1] = '\0';

      Serial.print("Email folder: ");                
      Serial.println(path);
    }
    else
    {
      Serial.println("Error getting the email");   
    }
    sendATcommand("AT+POP3OUT", "OK",10000);
  }

  if (answer == 1)
  {

    sendATcommand("AT+FSCD=C:/Email", "OK", 2000);
    sprintf(data, "AT+POP3READ=0,\"%s\"", path);

    // Clean the input buffer
    while( Serial.available() > 0) Serial.read();   

    // Send the AT command 
    Serial.println(data);    

    counter = 0;
    previous = millis();
    answer = 0;
    data[0] = '\0';
    // this loop waits for the answer
    do{

      if(Serial.available() != 0){    
        data[counter] = Serial.read();
        counter++;
        // check if "OK" is in the response of the module
        if (strstr(data, "OK") != NULL)    
        {
          answer = 1;
        }

        // check if "ERROR" is in the response of the module
        if (strstr(data, "ERROR") != NULL)    
        {
          answer = 2;
        }
      }
      // Waits for the answer with time out
    }
    while((answer == 0) && ((millis() - previous) < 10000) && (counter < 511));   

    data[counter] = '\0';
    if (answer == 1)
    {
      Serial.println("Email:");
      Serial.println(data);        
    }
    else if (counter == 511)
    {
      Serial.println("Buffer limit reached!! Data received:");
      Serial.println(data);       
    }
    else
    {
      Serial.println("Error");
    }


  }



}
void loop(){

}

void power_on(){

  uint8_t answer=0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin,HIGH);
    delay(3000);
    digitalWrite(onModulePin,LOW);

    // waits for an answer from the module
    while(answer == 0){    
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1,
unsigned int timeout){

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while( Serial.available() > 0) Serial.read();    // Clean the input buffer

  Serial.println(ATcommand);    // Send the AT command 


    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{

    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)    
      {
        answer = 1;
      }
    }
    // Waits for the answer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}
        
