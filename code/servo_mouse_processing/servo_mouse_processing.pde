/**
 * Servocontrol (derived from processing Mouse 1D example.) 
 * 
 * Updated 24 November 2007
 */


// Use the included processing code serial library
import processing.serial.*;        


int gx = 15;
int gy = 35;
int omx = 0;
int omy = 0;
int xpos=90;
int ypos=90;

float leftColor = 0.0;
float rightColor = 0.0;
Serial port;                         // The serial port



void setup() 
{
  size(720, 720);
  colorMode(RGB, 1.0);
  noStroke();
  rectMode(CENTER);
  frameRate(100);

  println(Serial.list()); // List COM-ports

  //select second com-port from the list
  port = new Serial(this, Serial.list()[0], 19200); 
}

void draw() 
{
  background(0.0);
  if(mouseX != omx) {
    updateX(mouseX); 
    fill(mouseX/4); 
    rect(150, 320, gx*2, gx*2);
    omx = mouseX;
  }

  if(mouseY != omy) {
    updateY(mouseY); 
    fill(180 - (mouseX/4)); 
    rect(450, 320, gy*2, gy*2);
    omy = mouseY;
  }
}

void updateX(int x) 
{
  //Calculate servo postion from mouseX
  xpos= x/4;

  //Output the servo position ( from 0 to 180)
  // 200 write x axis (servo 0)
  port.write(200);
  port.write(xpos);
   println("X:"+xpos+" S:"+port.read());

  // Just some graphics
  leftColor = -0.002 * x/2 + 0.06;
  rightColor =  0.002 * x/2 + 0.06;

  gx = x/2;
}

void updateY(int y) 
{
  //Calculate servo postion from mouseY
  ypos= y/4;

  //Output the servo position ( from 0 to 180)
  // 201 write y axis (servo 1)
  port.write(201);
  port.write(ypos);
  println("Y:"+ypos+" S:"+port.read());


  // Just some graphics
  leftColor = -0.002 * y/2 + 0.06;
  rightColor =  0.002 * y/2 + 0.06;

  gy = 100-y/2;
}
