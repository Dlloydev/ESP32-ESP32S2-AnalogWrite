/*
  Playing notes based on sliding pot position
  https://wokwi.com/projects/351175246893548120
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>

const int tonePin = 4;
const int potPin = 12;
const int notes = 8;
const note_t cScale[notes] = { NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_B, NOTE_C };
const int cScaleOctaves[notes] = { 4, 4, 4, 4, 4, 4, 4, 5 };
const int duration = 500;  // note duration ms
const int interval = 0;    // pause between notes ms

Pwm pwm = Pwm();  // constructor

void setup() {
  pinMode(potPin, INPUT); // sliding pot input
  pwm.attach(potPin, 2);  // attach to channel 2
}

void loop() {
  int sensorVal = analogRead(potPin); 
  int scale = map(sensorVal, 0, 4095, 0, notes - 1);
  note_t note = cScale[scale];
  int octave = cScaleOctaves[scale];
  pwm.note(tonePin, note, octave, duration, interval);  // auto attaches to ch 0
  delay(10);
}
