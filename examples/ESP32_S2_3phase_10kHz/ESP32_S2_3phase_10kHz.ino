 /*******************************************************
  This example demonstrates 3-phase, 10kHz PWM
  ESP32:    https://wokwi.com/projects/334722465700774482
  ESP32-S2: https://wokwi.com/projects/334765722024542804
  ESP32-C3: https://wokwi.com/projects/334856585002091092
  *******************************************************/

#include <pwmWrite.h>
const byte pwmPin[3] = {14, 13, 12};
const uint16_t duty = 341;
const byte deadtime = 21;
const uint16_t phase[3] = {0, 341, 682};
const uint16_t shift[3] = {42, 42, 0};
const uint32_t frequency = 10000;
const byte resolution = 10;
Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  pwm.pause();
  for (int i = 0; i < 3; i++) {
    pwm.write(pwmPin[i], (duty - deadtime), frequency, resolution, (phase[i] + shift[i]));
  }
  pwm.resume();
  pwm.printDebug();
}

void loop() {
}
