// Libraries
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <SPI.h>


char ssid[] = "Honor 6X";           // your network SSID (name)
char pass[] = "2121kufy";           // your network password

//char ssid[] = "De_Royal_Legacy_Hotel_2";           // your network SSID (name)
//char pass[] = "royal123";           // your network password

//char ssid[] = "De_Royal_Legacy_Hotel";           // your network SSID (name)
//char pass[] = "royal123";           // your network password

//char ssid[] = "kufy_LTE";           // your network SSID (name)
//char pass[] = "2121@kufy";           // your network password

int status = WL_IDLE_STATUS;   // the Wifi radio's status


WiFiEspClient client;        // Initialize the Ethernet client object

uint32_t ip;

void setup(void)
{
  // Initialize
  Serial1.begin(115200);
  Serial.begin(115200);

  WiFi.init(&Serial1);

  // attempt to connect to WiFi network

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  if (status == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
    Serial.println(F("======================================================"));
  }

  Serial.println(F("Setup ready"));
}

void loop(void)
{


  const int httpPort = 80;
  client.flush();
  client.stop();

  if (!client.connect("iris.kodehauz.xyz", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print(F("Sending request... "));

  while (client.connected()) {
    client.print(F("GET /api/display/2y10GSu7VyQ0HmgKHgfcDsrwe3tLLf7dDb0cfUnuuUfV8Q61t9GAyfC/message/next"));
    client.println(F(" HTTP/1.1"));

    client.println(F("Host: iris.kodehauz.xyz"));
    client.println(F("Connection: close\r\n"));
    client.println(F(""));

    while (client.available()) {
      //      Serial.println("something is here oooooh");
      delayMicroseconds(4060);
      String inByte = client.readStringUntil('\n');
      Serial.print(inByte);
    }


    //    Serial.println(F("done."));
  }

  //  }
  Serial.println(F(""));


  Serial.println(F("Closing connection"));
  Serial.println(F(""));

}
