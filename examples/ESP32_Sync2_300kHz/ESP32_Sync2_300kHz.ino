/*
  ESP32 2 Synchronized PWM Outputs (300kHz, 8-bit)
  https://wokwi.com/projects/349322326995632722
*/

#include <pwmWrite.h>
const byte pwm21 = 21;
const byte pwm19 = 19;
Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  pwm.pause();
  pwm.write(pwm21, 64, 300000);
  pwm.write(pwm19, 128, 300000);
  pwm.resume();
  pwm.printConfig();
}

void loop() {
}
