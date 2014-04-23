#include <toneAC.h>

// Melody liberated from the toneMelody Arduino example sketch by Tom Igoe.
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
int lowf = 13000;
int highf = 18000;

void setup() {} // Nothing to setup, just start playing!

void loop() {
  for (int repeat = 1; repeat <= 10; repeat += 1) {
    for (unsigned long freq = lowf; freq <= highf; freq += 10) {  
      toneAC(freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
      delay(1);     // Wait 1 ms so you can hear it.
    }
    for (unsigned long freq = highf; freq >= lowf; freq -= 10) {  
      toneAC(freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
      delay(1);     // Wait 1 ms so you can hear it.
    }
  }
  toneAC(); // Turn off toneAC, can also use noToneAC().

  delay(1000); // Wait a second.

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    toneAC(melody[thisNote], 10, noteDuration, true); // Play thisNote at full volume for noteDuration in the background.
    delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
  }

  while(1); // Stop (so it doesn't repeat forever driving you crazy--you're welcome).
}
