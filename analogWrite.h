#ifndef _ESP32_ESP32S2_ANALOG_WRITE_
#define _ESP32_ESP32S2_ANALOG_WRITE_
#include <Arduino.h>

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
#define NUM_OUTPUT_PINS  45
#define DAC1             17
#define DAC2             18
const uint8_t muxSize =  48;
const uint64_t pinMask = 0x7FE00207FFE; //PWM
#else
#define NUM_OUTPUT_PINS  34
#define DAC1             25
#define DAC2             26
const uint8_t muxSize =  40;
const uint64_t pinMask = 0x308EFF034; //PWM
#endif

typedef struct pinStatus {
  int8_t channel;
  int8_t pin;
  float frequency;
  uint8_t resolution;
  uint32_t value;
} pinStatus_t;

float analogWriteFrequency(int8_t pin, float frequency = 5000);
int32_t analogWriteResolution(int8_t pin, uint8_t resolution = 13);
void analogWrite(int8_t pin, int32_t value = 0);
int8_t getChannel(int8_t pin);
void printPinsStatus(void);

#endif
