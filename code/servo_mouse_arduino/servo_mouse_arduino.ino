#include <Servo.h>

Servo servox; Servo servoy;
int xPos = 90;
int yPos = 90;
int pauseD = 1000;
int panD = 25;

void setup() {

  pinMode(1,OUTPUT);
  servox.attach(0); //analog pin 0
  servox.write(xPos);
  //servo1.setMaximumPulse(2000);
  //servo1.setMinimumPulse(700);

  servoy.attach(1); //analog pin 1
  servoy.write(yPos);

}

void loop() {
  panX(9);
  delay(pauseD);
  panX(90);
  delay(pauseD);

  panY(1);
  delay(pauseD);
  panY(90);
  delay(pauseD);

  panY(165);
  delay(pauseD);
  panY(90);
  delay(pauseD);

  panX(162);
  delay(pauseD);
  panX(90);
  delay(pauseD);
} 

void panX(int x) {
  int dir = (x < xPos) ? -1 : 1;
  while(xPos != x) {
    xPos = xPos + dir;
    servox.write(xPos);
    delay(panD);
  }
}

void panY(int y) {
  int dir = (y < yPos) ? -1 : 1;
  while(yPos != y) {
    yPos = yPos + dir;
    servoy.write(yPos);
    delay(panD);
  }
}
