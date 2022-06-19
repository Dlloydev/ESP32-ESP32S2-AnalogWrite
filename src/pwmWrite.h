#pragma once
#ifndef _ESP32_PWM_WRITE_
#define _ESP32_PWM_WRITE_
#include <Arduino.h>
#include <driver/ledc.h>

class Pwm {

  public:

    Pwm();

    typedef struct pinStatus {
      uint8_t channel;
      uint8_t timer;
      uint8_t group;
      uint8_t pin;
      float frequency;
      uint8_t resolution;
      uint32_t duty;
      uint32_t phase;
    } pinStatus_t;

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3)
#define NUM_OUTPUT_PINS  45
    const uint8_t muxSize =  48;
#if (CONFIG_IDF_TARGET_ESP32C3)
    const uint64_t pinMask = 0xC03FF; // pwm pins
#else
    const uint64_t pinMask = 0x27FE00207FFE; // pwm pins
#endif
    pinStatus_t pinsStatus[8] = {
      {0, 0, 0, 255, 1000, 8, 0, 0 }, {2, 1, 0, 255, 1000, 8, 0, 0 },
      {4, 2, 0, 255, 1000, 8, 0, 0 }, {6, 3, 0, 255, 1000, 8, 0, 0 },
      {1, 0, 0, 255, 1000, 8, 0, 0 }, {3, 1, 0, 255, 1000, 8, 0, 0 },
      {5, 2, 0, 255, 1000, 8, 0, 0 }, {7, 3, 0, 255, 1000, 8, 0, 0 }
    };
    const uint8_t chd = 1;
#else //ESP32
#define NUM_OUTPUT_PINS  34
    const uint8_t muxSize =  40;
    const uint64_t pinMask = 0x308EFF034; // pwm pins
    pinStatus_t pinsStatus[8] = {
      { 0, 0, 0, 255, 1000, 8, 0, 0 }, { 2, 1, 0, 255, 1000, 8, 0, 0 },
      { 4, 2, 0, 255, 1000, 8, 0, 0 }, { 6, 3, 0, 255, 1000, 8, 0, 0 },
      { 8, 0, 1, 255, 1000, 8, 0, 0 }, {10, 1, 1, 255, 1000, 8, 0, 0 },
      {12, 2, 1, 255, 1000, 8, 0, 0 }, {14, 3, 1, 255, 1000, 8, 0, 0 }
    };
    const uint8_t chd = 2;
#endif

    float write(uint8_t pin, int32_t duty);
    float write(uint8_t pin, int32_t duty, float frequency);
    float write(uint8_t pin, int32_t duty, float frequency, uint8_t resolution);
    float write(uint8_t pin, int32_t duty, float frequency, uint8_t resolution, uint32_t phase);
    void detachPin(uint8_t pin, uint8_t ch);
    uint8_t getChannel(uint8_t pin);
    float writeFrequency(uint8_t pin, float frequency = 1000);
    int32_t writeResolution(uint8_t pin, uint8_t resolution = 8);
    void setPinsStatusDefaults(int32_t value = 0, float frequency = 1000, uint8_t resolution = 8, uint32_t phase = 0);
    void printPinsStatus(void);

  private:

};
#endif
