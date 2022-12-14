/*
  Controls servo position 0-180 degrees (auto attaches to ch 0).
  Controls LED intensity on ch 1 (has variable frequency from servo ch).
  Controls a piezo buzzer on ch 2 (non-blocking and unaffected by small delays).
  
  Use slider pot to vary LED intensity (updates after each servo cycle).
  Use slide switch to change servo speed, buzzer and LED mode. 

  https://wokwi.com/projects/350973592395055698
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>

const int servoPin = 5;
const int tonePin = 4;
const int modePin = 18;
const int ledPin = 19;
const int potPin = 12;


Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  pinMode(modePin, INPUT_PULLUP);
  pinMode(potPin, INPUT);

  pwm.attach(ledPin, 1);           // attach potPin to channel 1
  pwm.attach(servoPin, 2);         // attach servoPin to channel 2
}

void loop() {
  int val = analogRead(potPin);
  val = map(val, 0, 4095, 0, 255);
  pwm.write(ledPin, val);

  if (digitalRead(modePin)) {
    for (int pos = 0; pos <= 180; pos++) {  // go from 0-180 degrees
      pwm.writeServo(servoPin, pos);        // set the servo position (degrees)
      delay(15);
      pwm.tone(tonePin, 2000, 50, 0);       // 2 kHz "beep" at upper limit
    }
    for (int pos = 180; pos >= 0; pos--) {  // go from 180-0 degrees
      pwm.writeServo(servoPin, pos);        // set the servo position (degrees)
      delay(15);
      pwm.tone(tonePin, 40, 65535, 0);      // 40 Hz "buzz" while arm is lifting
    }
  } else {
    for (int pos = 0; pos <= 180; pos++) {
      pwm.writeServo(servoPin, pos);
      delay(10);
      pwm.tone(tonePin, (180 - pos) * 5 + 200, 500); // "siren"
    }
    for (int pos = 180; pos >= 0; pos--) {
      pwm.writeServo(servoPin, pos);
      delay(10);
      pwm.tone(tonePin, (180 - pos) * 5 + 200, 500); // "siren"
    }
  }
}
