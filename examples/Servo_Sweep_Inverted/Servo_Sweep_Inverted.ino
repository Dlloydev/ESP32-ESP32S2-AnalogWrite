/*
  Controls servo position from 0-180 degrees and back
  https://wokwi.com/projects/351967394028061269
  by dlloydev, December 2022.

  This demonstrates using an inverted PWM output for servo control.
  It allows using only one transistor to drive a higher voltage Servo control signal.
  No additional latency added as the signal is inverted by the timer peripheral.
  Choose/edit an attach function parameter to customize the signal configuration. 
*/

#include <Servo.h>

Servo myservo = Servo();

const int servoPin = 5, ch = 0;

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin, true);                // attach to first free channel, inverted pwm
  //myservo.attach(servoPin, 3, true);             // attach to channel 3, inverted pwm
  //myservo.attach(servoPin, 6, 500, 2500, true);  // attach to ch 6, minUs=500 maxUs=2500, inverted pwm 
  myservo.printDebug();
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) {
    myservo.write(servoPin, pos);
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos--) {
    myservo.write(servoPin, pos);
    delay(15);
  }
}
