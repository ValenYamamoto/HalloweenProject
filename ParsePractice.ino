#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial Genotronex(0, 1); // RX, TX
String inputString = "";
float x = 0;
float y = 0;
String test = "{\"X\":0.123,\"Y\":-0.222 n}";

void setup() {
  // put your setup code here, to run once:
  inputString.reserve(200);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  getParams();
}

void parseCommand() {
  // **************************************** Reading strings from Serial Comms
  StaticJsonBuffer<100> commandBuffer;
  JsonObject& command = commandBuffer.parseObject(inputString);
  x = command["X"];
  y = command["Y"];
  Serial.print(x);
  Serial.print("  ");
  Serial.println(y);
}

void getParams() {
  // **************************************** calls parseCommand when all chars are in
   if (Serial.available()>0) {
    char inChar = (char)Serial.read();

    if (inChar == 'n') {
      parseCommand();
      inputString = "";
    } else {
      inputString += inChar;

    }
    Serial.println(inputString);
  }
}
