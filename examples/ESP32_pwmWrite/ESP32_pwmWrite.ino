/*******************************************************************
  pwmWrite Phase Shift Example:
  This example demonstrates phase shifting PWM signals.
  Note: The LEDC hardware cannot precisely synchronize the timmers,
  so this isn't suited for motor control where you'll need to use
  the MCPWM hardware timers.
  Try on Wokwi ESP32: https://wokwi.com/projects/334722465700774482
            ESP32-S2: https://wokwi.com/projects/334765722024542804
  ******************************************************************/

#include <pwmWrite.h>

const byte phaseA = 14;
const byte phaseB = 13;
const byte phaseC = 12;
const float freq = 100;
const byte resolution = 10;

Pwm pwm = Pwm();

void setup() {
  Serial.begin(115200);
  //          pin, duty, freq, resolution, shift
  pwm.write(phaseA, 341, freq, resolution, 0);
  pwm.write(phaseB, 341, freq, resolution, 341);
  pwm.write(phaseC, 341, freq, resolution, 682);
  pwm.printPinsStatus();
}

void loop() {
}
