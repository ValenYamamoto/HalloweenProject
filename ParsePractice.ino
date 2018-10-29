#include <ArduinoJson.h>

SoftwareSerial Genotronex(5, 6); // RX, TX
String inputString = "";
int angle;
int speed;

void setup() {
  // put your setup code here, to run once:
  inputString.reserve(200);
  Serial.begin(9600);
  char json[] = "{\"hello\":\"Heyo\"}";
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& object = jsonBuffer.parseObject(json);
  const char* world = object["hello"];
  Serial.print(world);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void parseCommand() {
  // **************************************** Reading strings from Serial Comms
//  StaticJsonBuffer<100> commandBuffer;
//  JsonObject& command = commandBuffer.parseObject(inputString);
//  String cmd = command["cmd"];
//
//  if (cmd == "some_command_1") {
//    // perform some actions as a result of receive some_command_1
//  } else if (cmd == "some_command_2") {
//    // perform some actions as a result of receive some_command_2
//  } else if (cmd == "seek_bar_command") {
//     // get seek bar value
//     int value = command["params"]["value"];
//     // actions ...
//  } else if (cmd == "256") {
//     speed = command["params"]["speed"];
//     angle = command["params"]["angle"];
//  }
}
