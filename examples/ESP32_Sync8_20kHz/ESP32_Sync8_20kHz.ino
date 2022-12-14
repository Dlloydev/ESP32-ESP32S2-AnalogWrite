/*
  ESP32 8 Synchronized PWM Outputs (20kHz, 10-bit)
  https://wokwi.com/projects/349319723103552084
*/

#include <pwmWrite.h>
const byte pwmPin[8] = {21, 19, 18, 5, 16, 4, 2, 15};
const uint16_t duty[8] = {64, 128, 192, 256, 320, 384, 448, 512};
const uint16_t phase[8] = {870, 870, 580, 580, 290, 290, 0, 0};
const uint32_t frequency = 20000;
const byte resolution = 10;
Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  pwm.pause();
  for (int i = 0; i < 8; i++) {
    pwm.write(pwmPin[i], duty[i], frequency, resolution, phase[i]);
  }
  pwm.resume();
  pwm.printConfig();
}

void loop() {
}
