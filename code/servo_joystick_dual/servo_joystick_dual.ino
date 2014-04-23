#include <Arduino.h>
#include <Servo.h>

Servo servoX;
Servo servoY;

// digital servo pins
byte xServoPin = 2;
byte yServoPin = 3;

// analog joystick pins
byte xJoystickPin = 0;
byte yJoystickPin = 1;
byte bJoystickPin = 2;

// led indicator pin
byte ledPin = 13;

// ongoing servo positions
byte servoCoords[2];

// mode
boolean rawMode = false;

// effective ranges of devices
//  joystick X & Y axis ranges
int jxMin = 0;
int jxMax = 1023;
int jyMin = 69;
int jyMax = 1023;
//  servo X & Y (pan & tilt) ranges
int sxMin = 9;
int sxMax = 162;
int syMin = 1;
int syMax = 165;

void setup()
{
  pinMode(xServoPin, OUTPUT);
  servoX.attach(xServoPin);
  pinMode(yServoPin, OUTPUT);
  servoY.attach(yServoPin);

  // analog pin numbers start at 14
  pinMode(xJoystickPin + 14, INPUT);
  pinMode(yJoystickPin + 14, INPUT);
  pinMode(bJoystickPin + 14, INPUT);

  // indicator LED
  pinMode(ledPin,OUTPUT);

  // Start serial communications for feedback
  Serial.begin(9600);
}

void loop()
{
  int joystick[3];
  boolean bDown = false;
  byte newCoords[2];

  readJoystick(joystick);
  if(testValHigh(joystick[2])) {
    if(!(bDown)) { // change mode once
      rawMode = toggleLed(rawMode);
      bDown = true;
    }
  } else {
    bDown = false;
    if(rawMode) {
      newCoords[0] = newRaw(joystick[0]);
      newCoords[1] = newRaw(joystick[1]);
    } else {
      newCoords[0] = newInc(joystick[0],servoCoords[0]);
      newCoords[1] = newInc(joystick[1],servoCoords[1]);
    }
  }
  writeServo(newCoords);
}

void writeServo(byte nc[2]) {
  if(nc[0] != servoCoords[0]) {
    servoCoords[0] = nc[0];
    servoX.write(servoCoords[0]);
  }
  if(nc[1] != servoCoords[1]) {
    servoCoords[1] = nc[1];
    servoX.write(servoCoords[1]);
  }
}

void readJoystick(int j[]) {
  j[0] = analogRead(xJoystickPin);
  j[1] = analogRead(yJoystickPin);
  j[2] = analogRead(bJoystickPin);
}

byte newRaw(int jc) {
  byte nc;
  float p;
  p = (0.0 + jc) / 1024.0;
  nc = 180 * p;
  // test new value just in case - byte should never be < 0
  if(nc > 180) {
    nc = 180;
  }
  return nc;
}

byte newInc(int jc, int sc) {
  if(testValHigh(jc)&&(sc<180)) {
    sc++;
  } else if(testValLow(jc)&&(sc>0)) {
    sc--;
  }
  return sc;
}

boolean testValHigh(int val) {
  if(val>(512+128)) {
    return true;  // button is down
  } else {
    return false; // button is up
  }
}

boolean testValLow(int val) {
  if(val<(512-128)) {
    return true;  // button is down
  } else {
    return false; // button is up
  }
}

boolean toggleLed(boolean lv) {
  lv = !(lv);
  if(lv) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  return lv;
}
