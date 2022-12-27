/***********************
   pwmWrite.h
   by dlloydev, 04/2021.
************************/

#pragma once
#ifndef _ESP32_PWM_WRITE_
#define _ESP32_PWM_WRITE_
#include <Arduino.h>
#include <driver/ledc.h>

class Pwm {

  public:

    Pwm();

#if (defined(CONFIG_IDF_TARGET_ESP32))
    const uint64_t pinMask = 0x308EFF034; // pwm pins
    const uint8_t chMax = 16;
    const uint8_t widthMax = 20;

#elif (defined(CONFIG_IDF_TARGET_ESP32S2) || (CONFIG_IDF_TARGET_ESP32S3))
    const uint64_t pinMask = 0x27FE00207FFE; // pwm pins
    const uint8_t chMax = 8;
    const uint8_t widthMax = 14;

#elif (defined(CONFIG_IDF_TARGET_ESP32C3))
    const uint64_t pinMask = 0xC03FF; // pwm pins
    const uint8_t chMax = 6;
    const uint8_t widthMax = 14;
#endif

    typedef struct config {
      uint8_t pin;
      float frequency;       // pwm frequency (Hz)
      uint32_t duty;         // pwm steps or clock ticks, determines pulse width
      uint8_t resolution;    // bits, determines the number of steps in pwm period
      uint8_t mode;          // ledc speed mode (0 or 1)
      uint8_t timer;         // ledc timer (0-3)
      uint32_t phase;        // ledc hpoint value for shifting pulse start
      uint16_t servoMinUs;   // minimum servo pulse in μs
      uint16_t servoDefUs;   // default servo pulse in μs
      uint16_t servoMaxUs;   // maximum servo pulse in μs
      uint32_t startMs;      // for non-blocking timing purposes in ms
    } config_t;

    config_t config[16] = {  // channel config data (see above)
      {255, 1000, 0, 8, 0, 0, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 0, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 1, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 1, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 2, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 2, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 3, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 0, 3, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 0, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 0, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 1, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 1, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 2, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 2, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 3, 0, 544, 1472, 2400, 0 },
      {255, 1000, 0, 8, 1, 3, 0, 544, 1472, 2400, 0 }
    };

    // pwm
    float write(uint8_t pin, uint32_t duty);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);
    uint8_t attach(uint8_t pin); // attach pin to next free channel
    uint8_t attach(uint8_t pin, uint8_t ch, bool invert = false); // attach to specified ch with invert option 

    // servo
    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs, bool invert);
    float read(uint8_t pin);
    float readMicroseconds(uint8_t pin);
    uint32_t writeServo(uint8_t pin, float value);

    // tone and note
    uint8_t tone(uint8_t pin, uint32_t frequency, uint16_t duration = 500, uint16_t interval = 0);
    uint8_t note(uint8_t pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval);

    // common
    uint8_t attached(uint8_t pin);     // check if pin is attaced
    uint8_t attachedPin(uint8_t ch);   // get pin on specified channel
    void detach(uint8_t pin);          // detach pin
    bool detached(uint8_t pin);        // check if pin is detached
    void pause(uint8_t ch = 255);      // pause timer on all or specified channel
    void resume(uint8_t ch = 255);     // resume timer on all or specified channel
    void printConfig(void);            // print the status of all channels
    float setFrequency(uint8_t pin, uint32_t frequency = 1000);
    uint8_t setResolution(uint8_t pin, uint8_t resolution = 10);

  private:
    enum State { ready, play, stop };
    State state = ready;
    void ledc_attach_with_invert(uint8_t pin, uint8_t ch, bool invert = false);
    void config_servo(uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    void wr_ch_pair(uint8_t ch, uint32_t frequency, uint8_t resolution);
    void wr_duty(uint8_t ch, uint32_t duty);
    void wr_freq_res(uint8_t ch, uint32_t frequency, uint8_t resolution);
    void wr_phase(uint8_t ch, uint32_t duty, uint32_t phase);
    void reset_fields(uint8_t ch);
    bool sync = false;
};
#endif
