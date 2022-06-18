#include <pwmWrite.h>
#define ledPin 4

int brightness = 0;
int step = 1;

Pwm pwm = Pwm();

void setup() {
}

void loop() {
  pwm.write(ledPin, brightness);
  brightness += step;
  if ( brightness >= 1023) step = -1;
  if ( brightness <= 0) step = 1;
  delay(1);
}
