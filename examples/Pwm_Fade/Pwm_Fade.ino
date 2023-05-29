/*
  PWM Fade
  https://wokwi.com/projects/349317263481897554
*/

#include <pwmWrite.h>
const byte ledPin = 4;
int brightness = 0;
int step = 1;
Pwm pwm = Pwm();

void setup() {
}

void loop() {
  pwm.write(ledPin, brightness);
  brightness += step;
  if ( brightness >= 255) step = -1;
  if ( brightness <= 0) step = 1;
  delay(2);
}
