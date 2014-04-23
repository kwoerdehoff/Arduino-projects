//This code works with a DHT11 humidity/ temperature sensing module
//from nuelectronics.com, complied with vers 0018 of the Arduino environment
//Sensor attached to P4 (nuelectronics shield)/ analog 0, aka digital 14.


//#define dht_PIN 0
#define dht_dpin 2

byte bGlobalErr;//for passing error code back from complex functions.
byte dht_dat[5];//Array to hold the bytes sent from sensor.

float lastHumidity;        //Added by DaveB
float lastTemperatureF;        //Added by DaveB
float lastTemperatureC;        //Added by DaveB

const byte mask = 0x7f;        //Added by DaveB
const int ledPin =  13;      // the number of the LED pin, added by DaveB

int ledState = LOW;             // ledState used to set the LED, added by DaveB

void setup(){
InitDHT();//Do what's necessary to prepare for reading DHT
Serial.begin(9600);
delay(1000);  //Let system settle, changed from 300 to 1000 by DaveB to allow
              //2 sec delay before addressing DHT22 again.
Serial.println("Humidity and temperature\n\n");
delay(1000);  //Wait rest of 1000ms recommended delay before accessing sensor,
              // changed from 700 to 1000 for a total of 2000 by DaveB

// set the digital pin as output to use the Arduino LED, Added by DaveB
pinMode(ledPin, OUTPUT);        //Added by DaveB
}//end "setup()"

void loop(){
  ReadDHT();//This is the "heart" of the program.
      //Fills global array dht_dpin[], and bGlobalErr, which
      //will hold zero if ReadDHT went okay.
      //Must call InitDHT once (in "setup()" is usual) before
      //calling ReadDHT.
  //Following: Display what was seen...
  switch (bGlobalErr){
     case 0:    //case 0 changed by DaveB

        //Changes added by DaveB as a result of Thomas Henry's article on the DHT22
        //in the March 2013 issue of Nuts&Volts.
        //
        //Values received from the DHT22 are in two bytes that represent 10 times the value.
        //Therefore 256 * first word plus second word provides 10 times the humidity or
        //temperature value.  MSB of dht_dat[2] tells us if the temperature is
        //positive or negative.  MSB value of 0 is pos temp, MSB value of 1 is a neg temp.

        lastHumidity = (dht_dat[0]*256+dht_dat[1]);  //Calculate float for 10*humidity
        lastHumidity = lastHumidity/10;  //Calculate humidity
	Serial.print("Humidity: ");
	Serial.print(lastHumidity);
        Serial.println("%  ");

        //dht_dat[2] = dht_dat[2] + 128;  // debug for showing negative temperature, uncomment to show neg temps
        // check for MSB set in dht_dat[2] which means negative temperature
        if ((dht_dat[2] & mask) != dht_dat[2]) {   //if they're different, it's negative
           dht_dat[2]= dht_dat[2] & mask;
           lastTemperatureC = -1*(dht_dat[2]*256+dht_dat[3]);  //Calculate neg float 10*temperature C
        }
        else {   //if they're not different, it's positive
           lastTemperatureC = (dht_dat[2]*256+dht_dat[3]);  //Calculate float 10*temperature C
        }

	lastTemperatureC = lastTemperatureC/10;  //Calculate temperature C
        Serial.print("Temp: ");
        Serial.print(lastTemperatureC);  //Uncomment these two lines for C temps
        Serial.print("C, ");             //Uncomment these two lines for C temps
        lastTemperatureF=lastTemperatureC*9/5+32;  //Calculate temperature F
        Serial.print(lastTemperatureF);
        Serial.println("F  ");
        Serial.println("");

         if (ledState == LOW)// alternately light and extinguish the LED
            ledState = HIGH;
         else
            ledState = LOW;
         digitalWrite(ledPin, ledState);

        break;
     case 1:
        Serial.println("Error 1: DHT start condition 1 not met.");
        break;
     case 2:
        Serial.println("Error 2: DHT start condition 2 not met.");
        break;
     case 3:
        Serial.println("Error 3: DHT checksum error.");
        break;
     default:
        Serial.println("Error: Unrecognized code encountered.");
        break;
     	}//end "switch"

  //delay changed from 800 to 2000 by DaveB
  delay(2000);//Don't try to access too frequently... in theory
                   //should be once per two seconds, fastest,
                   //but seems to work after 0.8 second.
}// end loop()

/*Below here: Only "black box" elements which can just be plugged unchanged
  unchanged into programs. Provide InitDHT() and ReadDHT(), and a function
  one of them uses.*/

void InitDHT(){
  	//DDRC |= _BV(dht_PIN);//set data pin... for now... as output
              //DDRC is data direction register for pins A0-5 are on
	//PORTC |= _BV(dht_PIN);//Set line high
              //PORTC relates to the pins A0-5 are on.
        //Alternative code...
//        if (dht_dpin-14 != dht_PIN){Serial.println("ERROR- dht_dpin must be 14 more than dht_PIN");};//end InitDHT
        pinMode(dht_dpin,OUTPUT);// replaces DDRC... as long as dht_dpin=14->19
        digitalWrite(dht_dpin,HIGH);//Replaces PORTC |= if dht_pin=14->19
}//end InitDHT

void ReadDHT(){
/*Uses global variables dht_dat[0-4], and bGlobalErr to pass
  "answer" back. bGlobalErr=0 if read went okay.
  Depends on global dht_PIN for where to look for sensor.*/
bGlobalErr=0;
byte dht_in;
byte i;
  // Send "start read and report" command to sensor....
  // First: pull-down i/o pin for 18ms
digitalWrite(dht_dpin,LOW);//Was: PORTC &= ~_BV(dht_PIN);
delay(18);
//db deleted following line
//delay(5);//TKB, from Quine at Arduino forum
/*aosong.com datasheet for DHT22 says pin should be low at least
  500us. I infer it can be low longer without any]
  penalty apart from making "read sensor" process take
  longer. */
//Next line: Brings line high again,
//   second step in giving "start read..." command
digitalWrite(dht_dpin,HIGH);//Was: PORTC |= _BV(dht_PIN);
delayMicroseconds(30);//DHT22 datasheet says host should
   //keep line high 20-40us, then watch for sensor taking line
   //low. That low should last 80us. Acknowledges "start read
   //and report" command.
   //Getting the delay exacly right is critical...
   //  put 40 in for the delay, and the program
   //  will work SOMETIMES, but not ALWAYS.
   //  You have to allow for processing overheads,
   //  etc, too. Using, say, 30 won't "magically"
   //  make the TOTAL delay (with overheads, etc)
   //  exactly 30.0000 milliseconds.

//Next: Change Arduino pin to an input, to
//watch for the 80us low explained a moment ago.
pinMode(dht_dpin,INPUT);//Was: DDRC &= ~_BV(dht_PIN);

delayMicroseconds(40);

//Read the LO
dht_in=digitalRead(dht_dpin);//Was: dht_in = PINC & _BV(dht_PIN);

if(dht_in){
   bGlobalErr=1;//Was: Serial.println("dht11 start condition 1 not met");
   return;
   }//end "if..."
delayMicroseconds(80);

dht_in=digitalRead(dht_dpin);//Was: dht_in = PINC & _BV(dht_PIN);

if(!dht_in){
   bGlobalErr=2;//Was: Serial.println("dht11 start condition 2 not met");
   return;
   }//end "if..."

/*After 80us low, the line should be taken high for 80us by the
  sensor. The low following that high is the start of the first
  bit of the forty to come. The routine "read_dht_dat()"
  expects to be called with the system already into this low.*/
delayMicroseconds(80);
//now ready for data reception... pick up the 5 bytes coming from
//   the sensor
for (i=0; i<5; i++)
   dht_dat[i] = read_dht_dat();

//Next: restore pin to output duties
pinMode(dht_dpin,OUTPUT);//Was: DDRC |= _BV(dht_PIN);
//N.B.: Using DDRC put restrictions on value of dht_pin

//Next: Make data line high again, as output from Arduino
digitalWrite(dht_dpin,HIGH);//Was: PORTC |= _BV(dht_PIN);
//N.B.: Using PORTC put restrictions on value of dht_pin

//Next see if data received consistent with checksum received
byte dht_check_sum =
       dht_dat[0]+dht_dat[1]+dht_dat[2]+dht_dat[3];
/*Condition in following "if" says "if fifth byte from sensor
       not the same as the sum of the first four..."*/
if(dht_dat[4]!= dht_check_sum)
   {bGlobalErr=3;}//Was: Serial.println("DHT11 checksum error");
};//end ReadDHT()

byte read_dht_dat(){
//Collect 8 bits from datastream, return them interpreted
//as a byte. I.e. if 0000.0101 is sent, return decimal 5.

//Code expects the system to have recently entered the
//dataline low condition at the start of every data bit's
//transmission BEFORE this function is called.

  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++){
      //We enter this during the first start bit (low for 50uS) of the byte
      //Next: wait until pin goes high
      while(digitalRead(dht_dpin)==LOW);//Was: while(!(PINC & _BV(dht_PIN)));
            //signalling end of start of bit's transmission.

      //Dataline will now stay high for 27 or 70 uS, depending on
            //whether a 0 or a 1 is being sent, respectively.
      delayMicroseconds(30);//AFTER pin is high, wait further period, to be
        //into the part of the timing diagram where a 0 or a 1 denotes
        //the datum being send. The "further period" was 30uS in the software
        //that this has been created from. I believe that a higher number
        //(45?) would be more appropriate.

      //Next: Wait while pin still high
      if (digitalRead(dht_dpin)==HIGH)//Was: if(PINC & _BV(dht_PIN))
 	   result |=(1<<(7-i));// "add" (not just addition) the 1
                      //to the growing byte
    //Next wait until pin goes low again, which signals the START
    //of the NEXT bit's transmission.
    while (digitalRead(dht_dpin)==HIGH);//Was: while((PINC & _BV(dht_PIN)));
    }//end of "for.."
  return result;
}//end of "read_dht_dat()"
