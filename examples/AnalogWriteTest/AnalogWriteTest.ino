// Reference: https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite

#include "analogWrite.h"

void setup() {
  Serial.begin(115200);

  analogWriteFrequency(23, 2300);
  analogWrite(23, 10);

  analogWriteFrequency(22, 125);
  analogWriteResolution(22, 3);
  analogWrite(22, 6);

  analogWriteFrequency(21, 125);
  analogWriteResolution(21, 3);
  analogWrite(21, 7);

  analogWriteFrequency(19, 125);
  analogWriteResolution(19, 3);
  analogWrite(19, 8);

  analogWriteFrequency(18, 1800);
  analogWriteResolution(18, 8);
  analogWrite(18, 4000);

  analogWriteFrequency(5, 500);
  analogWriteResolution(5, 12);
  analogWrite(5, 5000);

  analogWriteFrequency(17, 1700);
  analogWrite(17, 6000);

  pinMode(16, INPUT); //comment out to allow analogWrite on pin16
  analogWriteResolution(16, 10);
  analogWriteFrequency(16, 1600);
  analogWrite(16, 7000);

  pinMode(4, INPUT); //comment out to allow analogWrite on pin4
  analogWriteFrequency(4, 400);
  analogWriteResolution(4, 11);
  analogWrite(4, 8000);

  pinMode(2, INPUT); //comment out to allow analogWrite on pin2
  analogWriteFrequency(2, 200);
  analogWrite(2, 8191);

  Serial.println();

  printPinsStatus();
}

void loop() {
}
