/*
  Controls servo position by using a potentiometer
  https://wokwi.com/projects/350033311963284051
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>

Pwm pwm = Pwm();

const int potPin = 4;
const int servoPin = 5;
int val;

void setup() {
}

void loop() {
  val = analogRead(potPin);         // read the pot value (0-4095)
  val = map(val, 0, 4095, 0, 180);  // scale the pot reading to 0-180 (degrees)
  pwm.writeServo(servoPin, val);    // set the servo position (degrees)
  delay(15);
}
