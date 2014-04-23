#include <phi_interfaces.h>

#define btn_a 3
#define total_buttons 1
char mapping[]={'U','D'};
byte pins[] = {btn_a};
phi_button_groups my_btns(mapping, pins, total_buttons);

void setup(){
  Serial.begin(9600);
}

void loop(){
  char temp;
  temp=my_btns.getKey(); // Use phi_button_groups object to access the group of buttons
  if (temp!=NO_KEY) Serial.write(temp);
}
