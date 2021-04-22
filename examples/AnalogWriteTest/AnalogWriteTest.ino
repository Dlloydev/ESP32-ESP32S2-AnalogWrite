// Reference: https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite

#include "analogWrite.h"

void setup() {

  Serial.begin(115200);
  Serial.println();

  analogWriteFrequency(5, 100);
  analogWriteResolution(5, 10);
  analogWrite(5, 512);

  analogWrite(2, 100);
  analogWriteFrequency(2, 1);
  analogWriteResolution(2, 10);

  analogWriteResolution(21, 8);
  analogWriteFrequency(21, 2500);
  analogWrite(21, 254);

  analogWriteFrequency(12, 50);
  analogWriteResolution(12, 5);
  analogWrite(12, 20);

  analogWriteFrequency(13, 250);
  analogWriteResolution(13, 10);
  analogWrite(13, 511);

  pinMode(19, INPUT);
  analogWriteFrequency(19, 100);
  analogWriteResolution(19, 6);
  analogWrite(19, 44);

  analogWrite(22, 500);
  analogWriteFrequency(22, 1200);
  analogWriteResolution(22, 11);

  analogWrite(33, 650);
  analogWriteResolution(33, 10);
  analogWriteFrequency(33, 60);

  printPinsStatus();
}

void loop() {
}
