/*
  ESP32 3 Phase PWM Outputs (40kHz, 10-bit)
  https://wokwi.com/projects/349336125753524820
*/

#include <pwmWrite.h>
const byte pwmPin[3] = {21, 19, 18};
const uint16_t duty = 341;
const byte deadtime = 21;
const uint16_t phase[3] = {0, 341, 682};
const uint16_t shift[3] = {602, 602, 0};
const uint32_t frequency = 40000;
const byte resolution = 10;
Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  pwm.pause();
  for (int i = 0; i < 3; i++) {
    pwm.write(pwmPin[i], (duty - deadtime), frequency, resolution, (phase[i] + shift[i]));
  }
  pwm.resume();
  pwm.printConfig();
}

void loop() {
}
