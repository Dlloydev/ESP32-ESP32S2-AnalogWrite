/*
  ESP32 16 PWM Fade
  https://wokwi.com/projects/349232255258853970
*/

#include <pwmWrite.h>
const byte pwmPin[16] = {15, 2, 4, 16, 5, 18, 19, 21, 22, 23, 32, 33, 27, 14, 12, 13};
const byte duty[16] = {1, 3, 5, 8, 12, 17, 25, 36, 49, 66, 100, 160, 220, 250, 254, 255};
Pwm pwm = Pwm();

void setup() {
}

void loop() {
  static byte j;
  j++;
  for (int i = 0; i < 16; i++) {
    pwm.write(pwmPin[i], duty[j & 15]);
    j++;
  }
  delay(50);
}
