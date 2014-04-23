#include <Servo.h>

Servo servo1, Servo servo2;

void setup() {
  servo1.attach(0);
  servo2.attach(1);
}

void loop() {
  servo1.write(0);
  servo2.write(180);
  delay(1500);
  servo1.write(180);
  servo2.write(0);
  delay(1500);
}

