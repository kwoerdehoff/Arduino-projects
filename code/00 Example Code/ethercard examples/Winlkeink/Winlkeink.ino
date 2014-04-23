/*
Winlkeink
August 2012
For feedback, comments and questions go to winkleink.blogspot.com

Script to allow the controling on the Arduino over Ethernet using an ENC28J60 Ethernet socket and the Ethercard library
Assigning Static IP for the Arduino so I can know exactly which Arduino I am controlling
For this example the request is either http://192.168.1.5/?LED2=ON or http://192.168.1.5/?LED2=OFF
These can be called directly but then the Arduino would have to be the web server and present back the web page
with the option to turn the LED off or ON

For this example I am controlling the Arduino from a webserver on my PC (XAMP) using a PHP script.

*/
// I took took inspiration from the following 2 examples

// The BackSoon example provided with the EtherCard library
// Present a "Will be back soon web page", as stand-in webserver.
// 2011-01-30 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Example from the Internet
// https://github.com/lucadentella/enc28j60_tutorial/blob/master/_5_BasicServer/_5_BasicServer.ino
#include <EtherCard.h>

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,1,5 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

// Using a Variable for the Pin, but it is not necessary 
const int ledPin2 = 2;
const int ledPin4 = 4;


// Some stuff for responding to the request
char* on = "ON";
char* off = "OFF";
char* statusLabel;
char* buttonLabel;

// Small web page to return so the request is completed
char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Arduino 192.168.1.5"
  "</title></head>"
  "<body>"
    "<h3>Arduino 192.168.1.5</h3>"
  "</body>"
"</html>"
;

void setup(){
// Set Pin2 to be an Output
  pinMode(ledPin2, OUTPUT);
// Set Pin4 to be an Output
  pinMode(ledPin4, OUTPUT);

// Scary complex intializing of the EtherCard - I don't understand this stuff (yet0  
  ether.begin(sizeof Ethernet::buffer, mymac);
// Set IP using Static
  ether.staticSetup(myip, gwip);
}

void loop(){
  
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

// IF LED2=ON turn it ON
  if(strstr((char *)Ethernet::buffer + pos, "GET /?LED2=ON") != 0) {
      Serial.println("Received ON command");
      digitalWrite(ledPin2, HIGH);
    }

// IF LED2=OFF turn it OFF  
    if(strstr((char *)Ethernet::buffer + pos, "GET /?LED2=OFF") != 0) {
      Serial.println("Received OFF command");
      digitalWrite(ledPin2, LOW);
    }

// IF LED4=ON turn it ON
  if(strstr((char *)Ethernet::buffer + pos, "GET /?LED4=ON") != 0) {
      Serial.println("Received ON command");
      digitalWrite(ledPin4, HIGH);
    }

// IF LED4=OFF turn it OFF  
    if(strstr((char *)Ethernet::buffer + pos, "GET /?LED4=OFF") != 0) {
      Serial.println("Received OFF command");
      digitalWrite(ledPin4, LOW);
    }

//Return a page so the request is completed.

    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  
}
