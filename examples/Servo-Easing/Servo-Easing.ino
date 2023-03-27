/*
  Controls three servos with different easing settings
  https://wokwi.com/projects/360276061783595009
  by dlloydev, March 2023.

  ⚪ The green servo moves from 0 to 90 deg at 70 deg/s with linear motion.
  ⚪ The orange servo moves from 0 to 180 deg at 140 deg/s with sigmoid motion.
  ⚪ The purple servo moves from 45 to 135 deg at 180 deg/s with steep sigmoid motion.
*/

#include <pwmWrite.h>

const int servoPin1 = 21;
const int servoPin2 = 22;
const int servoPin3 = 23;

float speed1 = 70.0;
float speed2 = 140.0;
float speed3 = 180.0;

uint32_t prevMs1, prevMs2, prevMs3;
uint8_t pos1 = 90;
uint8_t pos2 = 180;
uint8_t pos3 = 135;

float dur1 = 90.0 / speed1 * 1000.0;
float dur2 = 180.0 / speed2 * 1000.0;
float dur3 = 90.0 / speed3 * 1000.0;

Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t ms1 = millis();
  uint32_t ms2 = ms1;
  uint32_t ms3 = ms1;
  float ye1, ye2, ye3;

  if (ms1 - prevMs1 > dur1) {
    prevMs1 = ms1;
    pos1 = (pos1 == 0) ? 90 : 0;
  }
  if (ms2 - prevMs2 > dur2) {
    prevMs2 = ms2;
    pos2 = (pos2 == 0) ? 180 : 0;
  }
  if (ms3 - prevMs3 > dur3) {
    prevMs3 = ms3;
    pos3 = (pos3 == 45) ? 135 : 45;
  }
  ye1 = pwm.writeServo(servoPin1, pos1, speed1, 0.0);
  ye2 = pwm.writeServo(servoPin2, pos2, speed2, 0.6);
  ye3 = pwm.writeServo(servoPin3, pos3, speed3, 0.8);
  Serial.print(ye1);
  Serial.print(",");
  Serial.print(ye2);
  Serial.print(",");
  Serial.println(ye3);
  delay(6);
}
