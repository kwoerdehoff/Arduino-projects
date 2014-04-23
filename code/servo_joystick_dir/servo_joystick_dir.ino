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

int sx = 90;
int sy = 90;

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
      sx = 90;
      sy = 90;
      bDown = true;
    }
    Serial.print(" - button pressed");
  } else {
    bDown = false;

    // x increment (left)
    if(jx>600) {
      sx++;
      if(sx > 179) {
        sx = 179;
      }
    }
    // x decrement (right)
    if(jx<400) {
      sx--;
      if(sx < 1) {
        sx = 1;
      }
    }
    // y increment (up)
    if(jy>600) {
      sy++;
      if(sy > 179) {
        sy = 179;
      }
    }
    // y decrement (down)
    if(jy<400) {
      sy--;
      if(sy < 1) {
        sy = 1;
      }
    }
  }

  servoX.write(sx);
  servoY.write(sy);

  Serial.print(", sx:");
  Serial.print(sx,DEC);
  Serial.print(", sy:");
  Serial.print(sy,DEC);
  Serial.println("");

  delay(10);
}

void center()
{
  servoX.write(90);
  servoY.write(90);
  delay(200); // wait for servos to center
}
