//
// original project by Sebastiaan Ebeltjes (domoticx.nl)
//
// Hardware: Arduino  (Mega 2650) + ESP8266 module or other Arduino : adapt pint to used arduino!
//  
// -----------------------------
// | 1 2                       |
// | 3 4     component side    |
// | 5 6        ESP8266        |
// | 7 8                       |
// ----------------------------- 
// 
// 1 - Mega 2650 pin 18 - TX1
// 2 - Gnd
// 3 - 3v3
// 4 - nc
// 5 - nc
// 6 - nc
// 7 - 3v3
// 8 - Mega 2650 pin 19 - RX1
//
// tried several wifi projects but all failed to complile due to errors in the WIFI6266 libraries
// (if sombody has a working set availible, pse let me know where to find it :-)
// found a webserver example by Sebastiaan Ebeltjes (domoticx.nl) and used this as starting point
// Did some rough programming to send an SMTP email
// To run the program, adapt to your WiFi settings (ssid and ssid-password), change to your own SMPT-server,
// change the email addresses used to valid email addresses
// After sending the email the program goes in an idle state. Adapt this to your wishes tot triger the email sending


#include <SoftwareSerial.h>

char aux_str[129];

String SndTxt = "";
unsigned int lastStringLength = SndTxt.length();     // previous length of the String

char buf[150];

boolean viewdata = true;
boolean authentication = false;
boolean sendemail = true;
int casevar = 1;

// WiFi settings SSID & pwd
const char  SSID_string[ ] = "MyWifi"; // just change MyWifi into your ssid
const char  PASS_string[ ] = "MyPwd";  // just change MyPwd into the ssid pwd

String Mailuser = ("dXNlcmlk"); //base64 userid
String Mailpwd = ("cGFzc3dvcmQ=");  // base 64 pwd


const char smtp_server[ ] = "smtp.orange.fr";      // SMTP server
const char smtp_protocol[ ] = "TCP"; 
const char smtp_port[ ] = "25";  

// mail sender and receipients
String MailFrom = "MAIL FROM: <hans@mymail.com>\r\n"; // change to valid sender email address
String HeaderFrom = "From: Hans <hans@mymail.com>\r\n"; // change to valid sender email address
String MailRCPT1 = "RCPT TO: <john@hismail.com>\r\n"; // 2 separate lines or like RCPT TO: friend@hotmail.com, friend2@yahoo.com
String HeaderTo1 = "To: john@hismail.com\r\n";
String MailRCPT2 = "RCPT TO: <ronald@anothermail.com>\r\n";
// String MailRCPT2 = ""; // to leave out this 2nd receiver
String HeaderTo2 = "CC: ronald@anothermail.com\r\n";
  
void setup() {
  Serial.begin(115200); // Start de seriele monitor op 9600 baud.
  Serial1.begin(19200); // Start de software monitor op 9600 baud.
}

void loop() {
  // we end execution op sending email if something is not going up to plan....
  switch (casevar) {
    case 0:
      Serial.println("Error during sending email");
      delay(10000);
      casevar = 100;
      break;
    case 1:
      Serial.println("\r\n----- [ Snelheid ESP naar 115200 bps ] -----a");
      sendcommand("AT+CIOBAUD=115200\r\n", "OK", 1000, viewdata); 
      Serial1.end();
      Serial1.begin(115200); // Start de software monitor op 115200 baud.
      casevar = 2;
      break;
    case 2:
      Serial.println("\r\n----- [ check if ESP reacts ] -----");
      if (sendcommand("AT\r\n", "OK\r\n", 3000, viewdata) == 1) {casevar = 3;} else {casevar = 0;};
      break;
    case 3:
      Serial.println("\r\n----- [ disconnect WiFi ] -----");
      sendcommand("AT+CWQAP\r\n", "OK\r\n", 3000, viewdata); 
      casevar++;
      break;
    case 4:
      Serial.println("\r\n----- [ disconnect WiFi ] -----");
      sendcommand("AT+CWQAP\r\n", "OK\r\n", 3000, viewdata);
      casevar++;
      break;
     case 5:  
       Serial.println("\r\n----- [ RESET DE MODULE (RST) ] -----");
       sendcommand("AT+RST\r\n", "ready\r\n", 8000, viewdata);
       casevar++;
       break;
     case 6:  
       Serial.println("\r\n----- [ disconnect WiFi ] -----");
       if (sendcommand("AT+CWQAP\r\n", "OK\r\n", 3000, viewdata) == 1) {casevar++;} else {casevar = 0;};
       break;
     case 7:  
       Serial.println("\r\n----- [ ZET ESP IN STATION MODE (CWMODE) ] -----");
       if (sendcommand("AT+CWMODE=1\r\n", "OK\r\n", 2000, viewdata) == 1) {casevar++;} else {casevar = 0;};
       break;
     case 8:
       Serial.println("\r\n----- [ INLOGGEN OP WIFI (CWJAP_CUR) ] -----");
       sprintf(aux_str, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", SSID_string , PASS_string); 
       if (sendcommand(aux_str, "\r\nOK\r\n", 10000, viewdata) == 1) {casevar++;} else {casevar = 0;};
       break;
     case 9:
       Serial.println("\r\n----- [IP ADRES] -----");
       if (sendcommand("AT+CIFSR\r\n", "OK", 2000, viewdata) == 1) {casevar++;} else {casevar = 0;};       
       break;
     case 10:
       Serial.println("Configuring Email..");
       sprintf(aux_str, "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", smtp_protocol, smtp_server, smtp_port); 
       if (sendcommand(aux_str, "server ready\r\n", 2000, viewdata) == 1) {casevar++;} else {casevar = 0;}; 
       break;
     case 11:
       Serial.println("\r\n----- [ SENDING MESSAGE IN 2 STEPS - 1rst length string; 2nd the string... ] -----");
       SndTxt = "HELO Maison Neuve\r\n"; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata) == 1) {casevar++;} else {casevar = 0;}; 
       break;
     case 12:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "meet you\r\n", 9000, viewdata)) {casevar++;} else {casevar = 0;}; 
       break;
     case 13:
       if (authentication) {casevar = 14;} else {casevar = 30;};
       break;
     case 14:
       // authentication ---- to be checked and senddata changed in sendcommand with updated case statements
       //see "http://base64-encoder-online.waraxe.us/"
       SndTxt = "AUTH PLAIN LOGIN GSSAPI DIGEST-MD5\r\n"; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       sendData(buf, 500, viewdata);
       delay(3000);
       sendData(SndTxt, 500, viewdata);
       SndTxt = Mailuser+"\r\n"; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       sendData(buf, 500, viewdata);
       delay(1000);
       sendData(SndTxt, 500, viewdata);
       SndTxt = Mailpwd+"\r\n"; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       sendData(buf, 500, viewdata);
       delay(1000);
       sendData(SndTxt, 500, viewdata);
       break;
     case 30:
       if (sendemail) {casevar = 31;} else {casevar = 100;};
       break;
     case 31:
       SndTxt = MailFrom; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 32:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "sender ok", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 33:
       SndTxt = MailRCPT1;
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 34:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "recipient ok", 5000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 35:
       if (MailRCPT2 == "") {
         casevar = casevar + 2;
         break;}
       SndTxt = MailRCPT2; 
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 36:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "recipient ok", 5000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 37:
       SndTxt = "DATA\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 38:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 5000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 39:
       SndTxt = HeaderFrom;
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 40:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 41:
       SndTxt = HeaderTo1;
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 42:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 43:
       if (MailRCPT2 == "") {
         casevar = casevar + 2;
         break;}
       SndTxt = HeaderTo2;
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 44:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 45:
       SndTxt = "Bcc: \r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 46:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 47:
       SndTxt = "Subject: test\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 48:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 49:
       SndTxt = "Testing Success\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 50:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 51:
       SndTxt = "Testing Success no 2 pse reply!!\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 52:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 53:
       SndTxt = "\r\n.\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 54:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 55:
       SndTxt = "QUIT\r\n";
       lastStringLength = SndTxt.length();
       sprintf(buf, "AT+CIPSEND=%d\r\n", lastStringLength);
       if (sendcommand(buf, "OK\r\n>", 3000, viewdata)) {casevar++;} else {casevar = 0;};
       break;
     case 56:
       SndTxt.toCharArray(buf, lastStringLength+1);
       if (sendcommand(buf, "SEND OK\r\n", 3000, viewdata)) {casevar = 100;} else {casevar = 0;};
       Serial.println("Email Sent...");
       break;
     case 100: // close connection with server (might have been done already....)
       sendcommand("AT+CIPCLOSE\r\n", "OK\r\n", 9000, viewdata);
       casevar++;
       break;
     case 101://close wifi connection
       if (sendcommand("AT+CWQAP\r\n", "OK\r\n", 9000, viewdata)) {casevar++;} else {casevar = 0;};
       casevar = 200;
       Serial.println("Done.... starting to do nothing usefull for the moment");
       break;
     case 200:
//       Serial.println("Ready");
       delay(10000);
       break;
     }
}

int8_t sendcommand(char* command, char* expected_answer, unsigned int timeout, boolean debug){

  uint8_t x=0,  answer=0;
  char response[1000];
  unsigned long previous;

  memset(response, '\0', 1000);    // Initialice the string

  while( Serial1.available() > 0) Serial1.read();    // Clean the input buffer
  if (debug) {
    Serial.print("--> sendcommand out : ");
    Serial.println(command);}
  Serial1.print(command);    // Send the command 

    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    // if there are data in the UART input buffer, reads it and checks for the answer
    if(Serial1.available() != 0){    
      response[x] = Serial1.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer) != NULL)    
      {
        answer = 1;
        if (debug) {
          Serial.print("response found : ");
          Serial.println(expected_answer);}
      }
    }
    // Waits for the answer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  if (debug) {
    Serial.println("--> sendcommand response from server : ");
    Serial.println(response);}
  return answer;
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  if (debug) {  Serial.print("--> Senddata : ");
  Serial.print(command);}
  Serial1.print(command); // Stuur een "lees" karakter naar de ESP.
  long int time = millis();
  if (debug) {Serial.println("--> Senddata - timeoutloop start");}
  
  while ( (time + timeout) > millis()) {
    while (Serial1.available()) { // De ESP heeft data om weer te geven, laat het zien in de serial monitor.
      char c = Serial1.read(); // Lees het volgende karakter.
      response += c;
    }
  }
  if (debug) {
    Serial.print("--> Senddata finished : ");
    Serial.println(response);
  }
  return response;
}
