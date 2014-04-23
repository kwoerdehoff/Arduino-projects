/*
||
|| @file Button.pde
|| @version 1.1
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Display the intuitive way of using a button when using this Hardware Abstraction class
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#include <Button.h>

//create a Button object at pin 12
/*
|| Wiring:
|| GND -----/ ------ pin 12
*/
int btnPin = 3;
int ledPin = 6;

Button button = Button(btnPin,PULLUP);

void setup(){
  pinMode(ledPin,OUTPUT); //debug to led 13
}

void loop(){
  if(button.isPressed()){
	digitalWrite(ledPin,HIGH);
  }else{
	digitalWrite(ledPin,LOW);
  }
}
