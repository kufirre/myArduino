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
const char smtp_server[ ] = "*********";      // SMTP server
const char smtp_user_name[ ] = "*********";   // SMTP user name
const char smtp_password[ ] = "*********";    // SMTP password
const char smtp_port[ ] = "***";              // SMTP server port

//Write here you SIM card data
const char pin_number[] = "1234";
const char apn[] = "web.gprs.mtnnigeria.net";
const char user_name[] = "web";
const char password[] = "web";

//Write here your information about sender, direcctions and names
const char sender_address[ ] = "*********";    // Sender address
const char sender_name[ ] = "*********";       // Sender name

const char to_address[ ] = "*********";        // Recipient address
const char to_name[ ] = "*********";           // Recipient name

//Write here the subject and body of the email
char subject[ ] = "Your subject";
const char body[ ] = "Hello! Testing email from Arduino using GSM shield!";


int8_t answer;
int onModulePin= 2;
char aux_str[128];


void setup(){

  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);   

  Serial.println("Starting...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin_number);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets the SMTP server and port
  sprintf(aux_str, "AT+SMTPSRV=\"%s\",%s", smtp_server, smtp_port);
  sendATcommand(aux_str, "OK", 2000);

  // sets user name and password
  sprintf(aux_str, "AT+SMTPAUTH=1,\"%s\",\"%s\"", smtp_user_name, smtp_password);
  sendATcommand(aux_str, "OK", 2000);

  // sets sender adress and name
  sprintf(aux_str, "AT+SMTPFROM=\"%s\",\"%s\"", sender_address, sender_name);
  sendATcommand(aux_str, "OK", 2000);

  // sets sender adress and name
  sprintf(aux_str, "AT+SMTPRCPT=1,0,\"%s\",\"%s\"", to_address, to_name);
  sendATcommand(aux_str, "OK", 2000);

  // subjet of the email
  sprintf(aux_str, "AT+SMTPSUB=\"%s\"", subject);
  sendATcommand(aux_str, "OK", 2000);

  // body of the email
  sprintf(aux_str, "AT+SMTPBODY=\"%s\"", body);
  sendATcommand(aux_str, "OK", 2000);


  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", user_name, password);
  sendATcommand(aux_str, "OK", 2000);

  delay(2000);

  Serial.println("Sending email..."); 
  // sends the email and waits the answer of the module
  answer = sendATcommand("AT+SMTPSEND", "+SMTP: SUCCESS", 60000);
  if (answer == 1)
  {
    Serial.println("Done!");    
  }
  else
  {
    Serial.println("Error");         
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
    while(answer == 0){     // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){

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
    // if there are data in the UART input buffer, reads it and checks for the answer
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
        
