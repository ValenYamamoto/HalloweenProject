#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define JoyX 0
#define JoyY 1
#define servoMax 550
#define servoMin 125
#define clawMax 547
#define clawMin 100


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int potValues[4];
int servoPos[4];
int lastPosClaw = 60;
int lastPos[4] = {90, 90, 90, 90};
int changeInPos[4] = {0, 0, 0, 0};
bool delayMove[4] = {false, false, false, false};
Servo servo[4];

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
//  servo[0].attach(Pin1);
//  servo[1].attach(Pin2);
//  servo[2].attach(Pin3);
//  servo[3].attach(Pin4);
  pwm.begin();
  pwm.setPWMFreq(60);
   yVal.reserve(10);
  xVal.reserve(10);
  Serial.begin(9600);
  Serial.println("Starting");
  servo[0].write(0);
  
}

void loop() {
  // put your main code here, to run repeatedly:
//  moveServos();
//  printPotValues();
//  Serial.println();
//  Serial.print("JoyX: ");
//  Serial.println(getJoyX());
//  Serial.print("JoyY: ");
//  Serial.println(getJoyY());
  parse();
  int degree = getDegree(x/5.0, y/5.0);
  int magnitude = getMagnitude(x/5.0, y/5.0);

  moveToDirectionDegrees(degreeToRadian(degree), magnitude);
  
  openClaw(true);
  
}

void moveServos() {
  getPotValues();
  moveServoPot(0);
  moveServoPot(1);
  moveServoPot(2);
  moveServoPot(3);
}

void moveServoPot(int servoNumber) {
  servo[servoNumber].write(potValues[servoNumber]);
}

void moveServo(int servoNumber, int value) {
//  servo[servoNumber].write(value);
  pwm.setPWM(servoNumber, 0, getPWMVal(value));
}

void getPotValues() {
  potValues[0] = mapPotValues(0);
  potValues[1] = mapPotValues(1);
  potValues[2] = mapPotValues(2);
  potValues[3] = mapPotValues(3);
}

void printPotValues() {
  Serial.print(mapPotValues(0));
  Serial.print("  ");
  Serial.print(mapPotValues(1));
  Serial.print("  ");
  Serial.print(mapPotValues(2));
  Serial.print("  ");
  Serial.println(mapPotValues(3));
}

int mapPotValues(int potNumber) {
  int val = analogRead(potNumber);
  val = map(val, 0, 1023, 0, 180);
  return val;
}

void moveToDirectionDegrees(double direct, int degree) {
  degree = constrain(degree, 0, 100);
  direct = constrain(direct, 0, 360);

  servoPos[0] = (int) (cos(direct) * degree) - 10;
  servoPos[1] = (int) (-sin(direct) * degree) - 10;
  servoPos[2] = (int) (-cos(direct) * degree);
  servoPos[3] = (int) (sin(direct) * degree) - 10;

  for (int i = 0; i < 4; i ++) {
    servoPos[i] += 90;
//    Serial.print("Servo Number: ");
//    Serial.print(i + 1);
//    Serial.print("  position: ");
//    Serial.println(servoPos[i]);
    servoPos[i] = constrain(servoPos[i], 0, 180);
    changeInPos[i] = servoPos[i] - lastPos[i];
    if (changeInPos[i] < 0) {
      delayMove[i] = true;
    }
  }
  
  for (int i = 0; i < 4; i ++) {
    if(delayMove[i] != true) {
      moveServo(i, servoPos[i]);
    }
  }

//  delay(10);
  
  for (int i = 0; i < 4; i ++) {
    if(delayMove[i] = true) {
      moveServo(i, servoPos[i]);
    }
  }
  
  for (int i = 0; i < 4; i ++) {
    lastPos[i] = servoPos[i];
  }
}

double getJoyX() {
  return deadZoneCorrection(convertFloat(analogRead(JoyX)));
}

double getJoyY() {
  return deadZoneCorrection(convertFloat(analogRead(JoyY)));
}

double convertFloat(int num) {
  return (((double) (num))/1021.0) * 2 - 1;
}

double deadZoneCorrection(double num) {
  if (abs(num) <= 0.1) {
    return 0;
  }
  return num;
}

double getDegree(double x, double y) {
  double degree = radiansToDegrees(atan(y/x));
  if (x < 0) {
    if (y > 0) {
      degree = 180 + degree; 
    } else {
      degree = degree + 180;
    }
  } else {
    if (y < 0) {
      degree = 360 + degree;
    }
  }
  degree = degree - 90;
  if (degree < 0) {
    degree = 360 + degree;
  }
  if (x == 0 && y == 0) {
    return 0;
  }
  return degree;
}

double radiansToDegrees(double radian) {
  return radian/3.14159 * 180;
}

double degreeToRadian(double degree) {
  return degree/180 * 3.14159;
}

double getMagnitude(double x, double y) {
  double magnitude = sqrt(pow(x, 2) + pow(y, 2));
  if (magnitude > 1) {
    magnitude = 1;
  }
  return magnitude * 90;
}

int getPWMVal(double pos) {
  int range = servoMax - servoMin;
  return pos/180.0 * range + servoMin;
}

void moveClawServo(int pos) {
  int range = clawMax - clawMin;
  pos = pos/180.0 * range + clawMin;
//  Serial.print("Claw Pos: ");
//  Serial.println(pos);
  pwm.setPWM(4, 0, pos);
}

void openClaw(bool button) {
  if (button) {
    if (lastPosClaw != 120) {
      moveClawServo(120);
      lastPosClaw = 120;
    }
  } else {
    if (lastPosClaw != 58) {
      moveClawServo(58);
      lastPosClaw = 58;
    }
  }
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
