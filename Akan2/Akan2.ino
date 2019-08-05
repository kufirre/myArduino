
#include <DHT.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>

const char* ssid     = "Honor 6X";
const char* password = "2121kufy";

//const char* ssid     = "De_Royal_Legacy_Hotel";
//const char* password = "royal123";

const char* host = "dweet.io";

#define DHTPIN 7
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHT11);

const char* thing  = "Akan_IoT_StreetLight";

bool flag = false;

int status = WL_IDLE_STATUS;   // the Wifi radio's status

void setup() {
  Serial.begin(115200);
  WiFi.init(&Serial);

  pinMode (8, OUTPUT);
  digitalWrite(8, HIGH);

  delay(10);

  dht.begin();

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, password);
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  Serial.println("");
//  Serial.print("connecting to ");
//  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiEspClient client;
  const int httpPort = 80;
    client.flush();
  //  client.stop();
  delay(500);

  if (!client.connect(host, httpPort)) {
//    client.stop();
//    delay(200);
    Serial.println("connection failed");
    flag = false;
    return;
  }
  flag = true;

//  while (flag) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String url = "/dweet/for/";
    url += thing;
    url += "?";
    url += "temp=";
    url += t;
    url += "&humidity=";
    url += h;

//    Serial.print("Requesting URL: ");
//    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: keep-alive\r\n\r\n");
//  Serial.println();
//  Serial.println("closing connection");

//  client.flush();
//  client.stop();
}
