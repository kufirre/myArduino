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

char server[] = "smtp.gmail.com";

int status = WL_IDLE_STATUS;   // the Wifi radio's status
int light = 12;


WiFiEspClient client;        // Initialize the Ethernet client object

uint32_t ip;

void setup(void)
{
  // Initialize
  Serial.begin(115200);
  pinMode (light, OUTPUT);
  digitalWrite(light, LOW);


  WiFi.init(&Serial);

  // attempt to connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //initialice and connect to wifi lan
  WiFi.begin(ssid, pass);

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




byte sendEmail()

{
  if (espClient.connect(server, 25) == 1)
  {
    Serial.println(F("connection SUCCESSFUL"));
  }
  else
  {
    Serial.println(F("connection FAILED"));
    return 0;
  }



  void loop(void)
  {
    //    delay(3000);
    // Measure from DHT
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    temperature = (int)t;
    humidity = (int)h;

    Serial.println(F("Measurements done"));

    const int httpPort = 80;
    //  client.connect("google.com", httpPort);
    client.flush();
    client.stop();

    if (!client.connect("dweet.io", httpPort)) {
      Serial.println("connection failed");
      return;
    }

    Serial.print(F("Sending request... "));
    bool  pinState = digitalRead(light);
    while (client.connected()) {
      client.print(F("GET /dweet/for/"));
      client.print(thing_name);
      client.print(F("?temperature="));
      client.print(temperature);
      client.print(F("&humidity="));
      client.print(humidity);
      client.print(F("&light1="));
      client.print(pinState);
      client.print(F("&light2="));
      client.print(pinState);
      client.print(F("&light3="));
      client.print(pinState);
      client.println(F(" HTTP/1.1"));

      client.println(F("Host: dweet.io"));
      client.println(F("Connection: keep-alive\r\n"));
      client.println(F(""));

      Serial.println(F("done."));
    }

    //  }
    Serial.println(F(""));


    //  Serial.println(F("Closing connection"));
    Serial.println(F(""));

  }
