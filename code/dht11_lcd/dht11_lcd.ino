// 
//   FILE:  dht11_test1.pde
// PURPOSE: DHT11 library test sketch for Arduino
//

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
        return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
        return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double RATIO = 373.15 / (273.15 + celsius);  // RATIO was originally named A0, possibly confusing in Arduino context
        double SUM = -7.90298 * (RATIO - 1);
        SUM += 5.02808 * log10(RATIO);
        SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
        SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM - 3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

/*
  The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD BL pin to digital pin 10
 * KEY pin to analogl pin 0
 */

#include <LiquidCrystal.h>
#include <dht11.h>

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

int adc_key_val[5] ={50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

String(dMsg);

dht11 DHT11;

#define DHT11PIN 2

void setup()
{
  lcd.clear(); 
  lcd.begin(16, 2);
  lcd.setCursor(0,0); 
  lcd.print("DHT 11 Testing");
}

void loop()
{
  adc_key_in = analogRead(0);    // read the value from the sensor 
  key = get_key(adc_key_in);  // convert into key press

  if (key != oldkey) {  // if keypress is detected
    delay(50);  // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor 
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) {
      oldkey = key;
      readDHT11PIN();
      switch(key) {
        case 1:
          dispMsg("% Hum: " + float2string((float)DHT11.humidity,2));
          break;
        case 2:
          dispMsg("Temp (C): " + float2string((float)DHT11.temperature,2));
          break;
        case 3:
          dispMsg("Temp (F): " + float2string(Fahrenheit(DHT11.temperature),2));
          break;
        case 4:
          dispMsg("Temp (K): " + float2string(Kelvin(DHT11.temperature),2));
          break;
        case 5:
          dispMsg("Dew Pt (C): " + float2string(dewPoint(DHT11.temperature, DHT11.humidity),2));
          break;

  // case 6:
  //dMsg = "DewPtFast (C): " + float2string(dewPointFast(DHT11.temperature, DHT11.humidity),2);

        default:
          // dMsg = "Lib Ver: " + DHTLIB_VERSION;
          dMsg = "Lib Ver: 0.4.1";
          break;
      }

    }
  }

  delay(2000);
}

void dispMsg(String lcdMsg) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
  lcd.print(lcdMsg);
}

// Convert ADC value to key number
int get_key(unsigned int input) {
  int k;
   
  for (k = 0; k < NUM_KEYS; k++) {
    if (input < adc_key_val[k]) {
      return k;
    }
  }
   
  if (k >= NUM_KEYS) k = -1;  // No valid key pressed
  return k;
}

void readDHT11PIN() {
  int chk = DHT11.read(DHT11PIN);

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);

  switch(chk)
  {
    case DHTLIB_OK: 
                  lcd.print("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                  lcd.print("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                  lcd.print("Time out error"); 
                break;
    default: 
                  lcd.print("Unknown error"); 
                break;
  }
}

String float2string(float fVal,int nDec) {
  String(sVal);
  int tVal;
  tVal = (int)fVal;
  sVal = "" + tVal;
  tVal = (int)(pow(10,nDec) * (fVal - tVal));
  sVal += tVal;
  return sVal;
}
//
// END OF FILE
//
