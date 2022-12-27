/*
  Playing Tones based on sliding pot position
  https://wokwi.com/projects/352178590336932865
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>

uint8_t tonePin = 4;
uint8_t potPin = 12;
uint16_t duration = 500;  // tone duration ms
uint16_t interval = 0;    // pause between tones ms

Pwm pwm = Pwm();  // constructor

void setup() {
  pinMode(potPin, INPUT); // sliding pot input
}

void loop() {
  uint16_t toneVal = analogRead(potPin);
  if (toneVal <= 4) toneVal = 4;
  pwm.tone(tonePin, toneVal, duration, interval);  // auto attaches to ch 0
  delay(10);
}
