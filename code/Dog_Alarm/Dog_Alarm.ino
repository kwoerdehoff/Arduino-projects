#include <toneAC.h>

// change this value (from 0-1) to change the sensitivity
//  note: most effective ranges are going to be in the 0.6-0.85 range
float sensitivity = 0.8; // set lower for more sensitivity

float alertLevel = 0.0; // weighted average

void setup() {
  // uncomment Serial lines for debugging data on serial port
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
}

void loop() {

  int sensorValue = digitalRead(2);

  
  alertLevel = (alertLevel + sensorValue) / 2;

  //Serial.println(alertLevel);

  if(alertLevel > sensitivity) {
      //playNotes(); // use this for human audible testing
      playSweep(10);
  }

  delay(10); // Wait a second.
}

void playSweep(int sndVol) {
  /**
    Species	Approximate Hearing Range (Hz) 
    human	64-23,000
    dog	        67-45,000 
    cat	        45-64,000 
  */
  int lowf = 15000;  // most humans won't hear this
  int highf = 20000; // many speakers cut off around here

  int stepRate = 200; // increase for longer sweep, decrease (above 0) for faster sweep
  int fstep = abs(highf - lowf) / stepRate;
  for (int repeat = 1; repeat <= 2; repeat += 1) {
    for (unsigned long freq = lowf; freq <= highf; freq += fstep) {  
      toneAC(freq,sndVol); // Play the frequency (low to high sweep in steps).
      delay(1);     // Wait 1 ms so you can hear it.
    }
    for (unsigned long freq = highf; freq >= lowf; freq -= fstep) {  
      toneAC(freq,sndVol); // Play the frequency (high to low sweep in steps).
      delay(1);     // Wait 1 ms so you can hear it.
    }
  }
  toneAC(); // Turn off toneAC, can also use noToneAC().
  delay(1000);
}

void playNotes() {
  // Melody liberated from the toneMelody Arduino example sketch by Tom Igoe.
  int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
  int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    toneAC(melody[thisNote], 10, noteDuration, true); // Play thisNote at full volume for noteDuration in the background.
    delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
  }
}

