#pragma once
#ifndef _ESP32_PWM_WRITE_
#define _ESP32_PWM_WRITE_
#include <Arduino.h>
#include <driver/ledc.h>

class Pwm {

  public:

    Pwm();

    typedef struct pinStatus {
      uint8_t pin;
      float frequency;
      uint32_t duty;
      uint8_t resolution;
      uint8_t mode;
      uint8_t channel;
      uint8_t timer;
      uint32_t phase;
    } pinStatus_t;

#if (defined(CONFIG_IDF_TARGET_ESP32))
    const uint64_t pinMask = 0x308EFF034; // pwm pins
    const uint32_t chMax = 16;

#elif (defined(CONFIG_IDF_TARGET_ESP32S2) || (CONFIG_IDF_TARGET_ESP32S3))
    const uint64_t pinMask = 0x27FE00207FFE; // pwm pins
    const uint32_t chMax = 8;

#elif (defined(CONFIG_IDF_TARGET_ESP32C3))
    const uint64_t pinMask = 0xC03FF; // pwm pins
    const uint32_t chMax = 6;
#endif

    pinStatus_t pinsStatus[16] = {
      {255, 1000, 0, 8, 0, 0, 0, 0 }, {255, 1000, 0, 8, 0, 1, 0, 0 },
      {255, 1000, 0, 8, 0, 2, 1, 0 }, {255, 1000, 0, 8, 0, 3, 1, 0 },
      {255, 1000, 0, 8, 0, 4, 2, 0 }, {255, 1000, 0, 8, 0, 5, 2, 0 },
      {255, 1000, 0, 8, 0, 6, 3, 0 }, {255, 1000, 0, 8, 0, 7, 3, 0 },
      {255, 1000, 0, 8, 1, 8, 0, 0 }, {255, 1000, 0, 8, 1, 9, 0, 0 },
      {255, 1000, 0, 8, 1, 10, 1, 0 }, {255, 1000, 0, 8, 1, 11, 1, 0 },
      {255, 1000, 0, 8, 1, 12, 2, 0 }, {255, 1000, 0, 8, 1, 13, 2, 0 },
      {255, 1000, 0, 8, 1, 14, 3, 0 }, {255, 1000, 0, 8, 1, 15, 3, 0 }
    };

    float write(uint8_t pin, uint32_t duty);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);
    void attachPin(uint8_t pin, uint8_t ch);
    void detachPin(uint8_t pin, uint8_t ch);
    void pause();
    void resume();
    uint8_t getChannel(uint8_t pin);
    float setFrequency(uint8_t pin, uint32_t frequency = 1000);
    uint32_t setResolution(uint8_t pin, uint8_t resolution = 8);
    void setPinsStatusDefaults(uint32_t duty = 0, uint32_t frequency = 1000, uint8_t resolution = 8, uint32_t phase = 0);
    void printPinsStatus(void);

  private:
    void configChannel(uint8_t ch);
    void timerPause(uint8_t ch);
    void timerResume(uint8_t ch);
    bool _sync = false;
};
#endif
