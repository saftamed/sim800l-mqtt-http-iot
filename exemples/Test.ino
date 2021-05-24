#include "mqtt.h"
#include <Arduino_JSON.h>
Mqtt mqtt(false);

void setup() {
  mqtt.begin(9600);
  mqtt.connect("8.tcp.ngrok.", "io", "15834",
               "cc",false/*, true, "use-token-auth", "g+fjh-hUsMx8lqw!2i"*/);
  delay(6000);
  mqtt.subscribe("iot-2/4561");
  delay(4000);
   mqtt.publish("iot-2/4561", "{\"action\":\"D\",\"pin\":13,\"value\":1,\"options\":50}");
Serial.println("");
}

void loop() {
  if (mqtt.available()) {
    String line = mqtt.readString();
    if (line == "ERROR") {
      Serial.println(line);
    } else {
      Serial.println(line);
     setActions(line);
    }
  }
}
/*

{
  "action":"D",
  "pin":12,
  "value":1,
  "options":50
}

*/
void setActions(String line){
  JSONVar myObject = JSON.parse(line);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  if((String)((const char*)myObject["ACTION"])=="D"){
    pinMode((int)myObject["PIN"],OUTPUT);
    digitalWrite((int)myObject["PIN"],(int)myObject["VALUE"]);
  }
}



