#include <Arduino.h>
#include <Servo.h>

Servo servoX;
Servo servoY;

int del = 0.1 * 1000;

// digital servo pins
int xServoPin = 2;
int yServoPin = 3;

// analog joystick pins
int xJoystickPin = 0;
int yJoystickPin = 1;
int bJoystickPin = 2;

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

  // Start serial communications for feedback
  Serial.begin(9600);
}

void loop()
{
  int jx,jy,jb;
  float px,py;
  int sx,sy;
  boolean bDown = false;

  // read joystick data
  jx = analogRead(xJoystickPin);
  jy = analogRead(yJoystickPin);
  jb = analogRead(bJoystickPin);
    Serial.print("jx:");
    Serial.print(jx,DEC);
    Serial.print(", jy:");
    Serial.print(jy,DEC);
    Serial.print(", jb:");
    Serial.print(jb,DEC);

  if(jb>500) { // button is pressed
    if(!(bDown)) { // only center once
      center();
      bDown = true;
    }
    Serial.print(" - button pressed");
  } else {
    bDown = false;
    // get joystick values as a value from 0 to 1
    //px = (0.0 + jx - jxMin) / (0.0 + jxMax - jxMin);
    //py = (0.0 + jy - jyMin) / (0.0 + jyMax - jyMin);
    px = (0.0 + jx) / (0.0 + 1024);
    py = (0.0 + jy) / (0.0 + 1024);
    Serial.print(", px:");
    Serial.print(px,4);
    Serial.print(", py:");
    Serial.print(py,4);

    // calculate servo values within their effective range
    //sx = ((sxMax - sxMin) * px) + sxMin;
    //sy = ((syMax - syMin) * py) + syMin;
    sx = (180 * px);
    sy = (180 * py);

    servoX.write(sx);
    
    servoY.write(sy);

    Serial.print(", sx:");
    Serial.print(sx,DEC);
    Serial.print(", sy:");
    Serial.print(sy,DEC);
  }
  Serial.println("");
  delay(100);
}

void center()
{
  servoX.write(90);
  servoY.write(90);
  delay(1000); // wait for servos to center
}
