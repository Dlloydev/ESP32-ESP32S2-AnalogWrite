/*
  ESP32 14 PWM Fade and 2 Servos
  https://wokwi.com/projects/349978851105833554
  by dlloydev, December 2022.
*/

#include <pwmWrite.h>
const byte pwmPin[16] = {15, 2, 4, 16, 5, 18, 19, 21, 22, 23, 32, 33, 27, 14, 12, 13};
const byte duty[16] = {1, 3, 7, 20, 40, 80, 100, 120, 135, 155, 175, 215, 235, 246, 253, 254};
Pwm pwm = Pwm();

void setup() {
}

void loop() {
  static byte j;
  (millis() & 0x1000) ? j++ : j--;
  for (int i = 0; i < 16; i++) {
    if (i > 1) pwm.write(pwmPin[i], duty[j & 15]);
    if (i == 0) pwm.writeServo(15, duty[j & 15]);
    if (i == 1) pwm.writeServo(2, duty[j & 15]);
    j--;
  }
  delay(30);
}
