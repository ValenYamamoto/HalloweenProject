#include <Wire.h>

float x;
float y;
String xVal = "";
String yVal = "";
String bVal = "";
bool getX = false;
bool getY = false;
bool getButton = false;
bool buttonVal = false;

void setup() {
  // put your setup code here, to run once:
  yVal.reserve(10);
  xVal.reserve(10);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  parse();
}

void parse() {
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();

    switch (inChar) {
      case '<':
        getX = true;
        break;
      case '>':
        getX = false;
        x = xVal.toInt();
        xVal = "";
        break;
      case '{':
        getY = true;
        y = yVal.toInt();
        yVal = "";
        break;
      case '}':
        getY = false;
        break;
      case '[':
        getButton = true;
        break;
      case ']':
        getButton = false;
        if(bVal.toInt() == 1) {
          buttonVal = true;
        } else {
          buttonVal = false;
        }
        bVal = "";
        break;
      case '1': 
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
      case '-':
        if (getX) {
          xVal += inChar;
        } else if (getY) {
          yVal += inChar;
        } else if(getButton) {
          bVal += inChar;
        }
        break;
    }
    
//    Serial.print("Xval: ");
//    Serial.print(xVal);
//    Serial.print("  Yval: ");
//    Serial.print(yVal);
    Serial.print("  X: ");
    Serial.print(x);
    Serial.print("  Y: ");
    Serial.print(y);
     Serial.print("  B: ");
    Serial.println(buttonVal);
  }
}
