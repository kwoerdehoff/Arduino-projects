/*
  MQ-2 Sensor connected to Arduino Uno
  Both A pins and left H pin connected to 5V out
  Right H pin connected to GND
  Both B pins conncted to 22k ohms resistor and A0
*/

int iSensorValue = 0;
int waterSensor = 3;
int MQD = 4;

void setup() {
  pinMode(waterSensor, INPUT);
  pinMode(MQD, INPUT);
  Serial.begin(9600); 
}

void loop() {
  // Read value on A0
  iSensorValue = analogRead(A1);
  //iSensorValue = digitalRead(waterSensor);
  
  // Display value
  Serial.print(F("MQ-2 Sensor Value = "));
  Serial.println(iSensorValue);
 
  // Loop 10 times per second
  delay(1000);
}
