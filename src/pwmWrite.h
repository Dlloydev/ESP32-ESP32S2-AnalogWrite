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
    const uint64_t pinMask = 0xFF0EEFFFFF; // pwm pins
    const uint8_t chMax = 16;
    const uint8_t widthMax = 20;

#elif (defined(CONFIG_IDF_TARGET_ESP32S2) || (CONFIG_IDF_TARGET_ESP32S3))
    const uint64_t pinMask = 0x3FFE043FFFFF; // pwm pins
    const uint8_t chMax = 8;
    const uint8_t widthMax = 14;

#elif (defined(CONFIG_IDF_TARGET_ESP32C3))
    const uint64_t pinMask = 0x3C07FF; // pwm pins
    const uint8_t chMax = 6;
    const uint8_t widthMax = 14;
#endif

    typedef struct mem {
      uint8_t pin;
      float frequency;       // pwm frequency (Hz)
      uint32_t duty;         // pwm steps or clock ticks, determines pulse width
      uint8_t resolution;    // bits, determines the number of steps in pwm period
      uint8_t mode;          // ledc speed mode (0 or 1)
      uint8_t timer;         // ledc timer (0-3)
      uint32_t phase;        // ledc hpoint value for shifting pulse start
      uint16_t servoMinUs;   // minimum servo pulse in μs (544)
      uint16_t servoMaxUs;   // maximum servo pulse in μs (2400)
      uint32_t startMs;      // servo easing or tone start ms
      uint32_t stopMs;       // servo easing stop ms value
      uint32_t deltaMs;      // servo easing ms difference
      uint32_t startDuty;    // servo easing start duty value
      uint32_t stopDuty;     // servo easing stop duty value
      uint32_t deltaDuty;    // servo easing duty difference
      float speed;           // servo easing speed (degrees per second)
      float ke;              // servo easing constant (0.0 linear, >0 and <1 sigmoid, 1.0 no easing)
      float te;              // servo easing normalized elapsed time (0.0 start - 1.0 stop)
      float ye;              // servo easing normalized position (0.0 start - 1.0 stop)
    } mem_t;

    mem_t mem[16] = {  // channel data (see above)
      {255, 1000, 0, 8, 0, 0, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 0, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 1, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 1, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 2, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 2, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 3, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 0, 3, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 0, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 0, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 1, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 1, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 2, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 2, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 3, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0},
      {255, 1000, 0, 8, 1, 3, 0, 544, 2400, 0, 0, 0, 0, 0, 0, 0, 1.0, 0, 0}
    };

    // pwm
    float write(uint8_t pin, uint32_t duty);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);
    
    // pwm
    uint8_t attach(uint8_t pin); // attach pin to next free channel
    uint8_t attach(uint8_t pin, uint8_t ch); // attach to specified ch
    uint8_t attachInvert(uint8_t pin); // attach pin to next free channel with inverted pwm
    uint8_t attachInvert(uint8_t pin, uint8_t ch); // attach to specified ch with inverted pwm

    // servo
    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t maxUs);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs);
    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t maxUs, float speed, float ke);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs, float speed, float ke);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs, float speed, float ke, bool invert);
    float read(uint8_t pin);
    float readMicroseconds(uint8_t pin);
    float writeServo(uint8_t pin, float value);
    float writeServo(uint8_t pin, float value, float speed, float ke);

    // tone and note
    void tone(uint8_t pin, uint32_t frequency, uint16_t duration = 500, uint16_t interval = 0);
    void note(uint8_t pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval);

    // common
    uint8_t attached(uint8_t pin);     // check if pin is attaced
    uint8_t attachedPin(uint8_t ch);   // get pin on specified channel
    void detach(uint8_t pin);          // detach pin
    bool detached(uint8_t pin);        // check if pin is detached
    void pause(uint8_t ch = 255);      // pause timer on all or specified channel
    void resume(uint8_t ch = 255);     // resume timer on all or specified channel
    void printDebug(void);             // print the status of all channels
    float setFrequency(uint8_t pin, uint32_t frequency = 1000);
    uint8_t setResolution(uint8_t pin, uint8_t resolution = 10);

  private:
    float duty2deg(uint8_t ch, uint32_t duty, float countPerUs);
    void ledc_attach_with_invert(uint8_t pin, uint8_t ch);
    void config_servo(uint8_t ch, uint16_t minUs, uint16_t maxUs, float speed = 0, float ke = 1.0);
    void wr_servo(uint8_t pin, float value, float speed, float ke);
    void wr_ch_pair(uint8_t ch, uint32_t frequency, uint8_t resolution);
    void wr_duty(uint8_t ch, uint32_t duty);
    void wr_freq_res(uint8_t ch, uint32_t frequency, uint8_t resolution);
    void wr_phase(uint8_t ch, uint32_t duty, uint32_t phase);
    void reset_fields(uint8_t ch);
    bool sync = false;
};
#endif
