#include <ArduinoJson.h>

char json[] = "{\"_token\": \"insert hashed key here\", \"id\": {\"title\": \"Play time\",\"day\": 9,\"month\": 5,\"year\": 2019,\"hour\": 18,\"minute\": 14,\"repeat\": true,\"repeat_period\": \"monthly\"}}";
StaticJsonDocument<1000> doc;


void setup() {

  Serial.begin(9600);
  auto error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }

  String _day = doc["id"]["day"];
  Serial.println(_day);
}

void loop() {}
