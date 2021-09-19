// Reference: https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite

#include "analogWrite.h"

void setup() {

  Serial.begin(115200);
  Serial.println(F("----------------"));

  // 3-phase PWM
  analogWrite(4, 341, 100, 10, 0);
  analogWrite(5, 341, 100, 10, 341);
  analogWrite(12, 341, 100, 10, 682);

  printPinsStatus();
}

void loop() {
}
