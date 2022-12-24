/*
  ESP32 16 PWM Fade
  https://wokwi.com/projects/349232255258853970
  by dlloydev, Koepel, November 2022.
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
    pwm.write(pwmPin[i], duty[j & 15]);
    j--;
  }
  delay(40);
}
