#include "analogWrite.h"
#define LED_BUILTIN 2

int brightness = 0;
int step = 1;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(LED_BUILTIN, 10);
}

void loop() {
  analogWrite(LED_BUILTIN, brightness);
  brightness += step;
  if ( brightness >= 1023) step = -1;
  if ( brightness <= 0) step = 1;
  delay(1);
}
