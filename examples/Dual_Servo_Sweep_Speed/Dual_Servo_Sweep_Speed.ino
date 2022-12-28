/*
  Controls 2 servo positions and speed from 0-180 deg and back
  https://wokwi.com/projects/351978833396630095
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>

Pwm pwm = Pwm();

const int speedPin1 = 32, speedPin2 = 12;
const int servoPin1 = 14, servoPin2 = 13;
float speed1, speed2, pos1, pos2;
bool dir1, dir2;

void setup() {
}

void loop() {
  speed1 = (analogRead(speedPin1)) / 4095.0;  // 0-100% speed
  speed2 = (analogRead(speedPin2)) / 2047.0;  // 0-200% speed

  pwm.writeServo(servoPin1, pos1);
  delay(5); // for simulator
  pwm.writeServo(servoPin2, pos2);
  delay(5); // for simulator

  if (pos1 < 0) pos1 = 0;
  if (pos2 < 0) pos2 = 0;

  if (dir1) {
    if (pos1 <= 180.0) {
      pos1 += speed1;
    } else {
      dir1 = false;
    }
  } else {
    if (pos1 >= 0.5) {
      pos1 -= speed1;
    } else {
      dir1 = true;
    }
  }
  if (dir2) {
    if (pos2 <= 180.0) {
      pos2 += speed2;
    } else {
      dir2 = false;
    }
  } else {
    if (pos2 >= 0.5) {
      pos2 -= speed2;
    } else {
      dir2 = true;
    }
  }
} // loop speed determines the maximum servo speed limit
