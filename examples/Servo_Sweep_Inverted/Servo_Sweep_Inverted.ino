/*
  Controls servo position from 0-180 degrees and back
  https://wokwi.com/projects/351967394028061269
  by dlloydev, December 2022.

  This demonstrates using an inverted PWM output for servo control.
  It allows using a only one transistor to drive a higher voltage Servo control signal.
  No additional latency is added like found with software inversion, because the inverted
  pulse remains at the start of the refresh period rather than being flipped to the end.
*/

#include <pwmWrite.h>

Pwm pwm = Pwm();

const int servoPin = 5, ch = 0;
bool invert = true;

void setup() {
pwm.attach(servoPin, ch, invert);
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) {  // go from 0-180 degrees
    pwm.writeServo(servoPin, pos);        // set the servo position (degrees)
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos--) {  // go from 180-0 degrees
    pwm.writeServo(servoPin, pos);        // set the servo position (degrees)
    delay(15);
  }
}