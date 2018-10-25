#include <Servo.h>
#define Pin1 9
#define Pin2 10
#define Pin3 11
#define Pin4 12
#define JoyX 4
#define JoyY 5

int potValues[4];
int servoPos[4];
Servo servo[4];

void setup() {
  // put your setup code here, to run once:
  servo[0].attach(Pin1);
  servo[1].attach(Pin2);
  servo[2].attach(Pin3);
  servo[3].attach(Pin4);
  Serial.begin(9600);
  Serial.println("Starting");
  servo[0].write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
//  moveServos();
//  printPotValues();
  Serial.print("Degree: ");
  Serial.println(getDegree());
  Serial.print("Magnitude: ");
  Serial.println(getMagnitude());
  Serial.println();
  moveToDirectionDegrees(getDegree(), getMagnitude());
  delay(1000);
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
  servo[servoNumber].write(value);
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
  degree = constrain(degree, 0, 90);
  direct = constrain(direct, 0, 360);

  servoPos[0] = (int) (cos(direct) * degree);
  servoPos[1] = (int) (-sin(direct) * degree);
  servoPos[2] = (int) (-cos(direct) * degree);
  servoPos[3] = (int) (sin(direct) * degree);
  
  for (int i = 0; i < 4; i ++) {
    servoPos[i] += 90;
    Serial.print("Servo Number: ");
    Serial.print(i + 1);
    Serial.print("  position: ");
    Serial.println(servoPos[i]);
    moveServo(i, servoPos[i]);
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

double getDegree() {
  double x = getJoyX();
  double y = getJoyY();
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

double getMagnitude() {
  double x = getJoyX();
  double y = getJoyY();
  double magnitude = sqrt(pow(x, 2) + pow(y, 2));
  if (magnitude > 1) {
    magnitude = 1;
  }
  return magnitude * 90;
}


