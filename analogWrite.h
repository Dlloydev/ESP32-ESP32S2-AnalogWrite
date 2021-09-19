#pragma once
#ifndef _ESP32_ESP32S2_ANALOG_WRITE_
#define _ESP32_ESP32S2_ANALOG_WRITE_

#include <Arduino.h>

#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))

namespace aw {

#include "driver/ledc.h"

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)

#define NUM_OUTPUT_PINS  45
#define DAC1             17
#define DAC2             18
const uint8_t muxSize =  48;
const uint64_t pinMask = 0x27FE00207FFE; //PWM

#else //ESP32
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
  uint32_t phase;
} pinStatus_t;

void awDetachPin(uint8_t pin, uint8_t ch);
int8_t awGetChannel(int8_t pin);

} //namespace aw

float analogWriteFrequency(int8_t pin, float frequency = 980);
int32_t analogWriteResolution(int8_t pin, uint8_t resolution = 8);
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution, uint32_t phase);
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution);
float analogWrite(int8_t pin, int32_t value, float frequency);
float analogWrite(int8_t pin, int32_t value);
void setPinsStatusDefaults(int32_t value = 0, float frequency = 980, uint8_t resolution = 8, uint32_t phase = 0);
void printPinsStatus(void);

#endif //ESP32 or ARDUINO_ARCH_ESP32
#endif //_ESP32_ESP32S2_ANALOG_WRITE_
