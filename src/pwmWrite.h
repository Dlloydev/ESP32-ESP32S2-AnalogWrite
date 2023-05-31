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
      int pin;
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
      double speed;          // servo easing speed (degrees per second)
      double ke;             // servo easing constant (0.0 linear, >0 and <1 sigmoid, 1.0 no easing)
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
    float write(int pin, uint32_t duty);
    float write(int pin, uint32_t duty, uint32_t frequency);
    float write(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
    float write(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);
    uint8_t attach(int pin);                 // attach pin to next free channel
    uint8_t attach(int pin, int ch);         // attach to specified ch
    uint8_t attachInvert(int pin);           // attach pin to next free channel with inverted pwm
    uint8_t attachInvert(int pin, int ch);   // attach to specified ch with inverted pwm

    // servo
    uint8_t attachServo(int pin);
    uint8_t attachServo(int pin, bool invert);
    uint8_t attachServo(int pin, int ch);
    uint8_t attachServo(int pin, int ch, bool invert);
    uint8_t attachServo(int pin, int minUs, int maxUs);
    uint8_t attachServo(int pin, int ch, int minUs, int maxUs);
    uint8_t attachServo(int pin, int ch, int minUs, int maxUs, bool invert);
    uint8_t attachServo(int pin, int minUs, int maxUs, double speed, double ke);
    uint8_t attachServo(int pin, int ch, int minUs, int maxUs, double speed, double ke);
    uint8_t attachServo(int pin, int ch, int minUs, int maxUs, double speed, double ke, bool invert);
    float read(int pin);
    float readMicroseconds(int pin);
    float writeServo(int pin, float value);
    float writeServo(int pin, float value, double speed, double ke);

    // tone and note
    void tone(int pin, uint32_t frequency, uint16_t duration = 500, uint16_t interval = 0);
    void note(int pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval);

    // common
    uint8_t attached(int pin);     // check if pin is attached
    uint8_t attachedPin(int ch);   // get pin on specified channel
    uint8_t firstFreeCh(void);     // get first free channel
    void detach(int pin);          // detach pin
    bool detached(int pin);        // check if pin is detached
    void pause(int ch = 255);      // pause timer on all or specified channel
    void resume(int ch = 255);     // resume timer on all or specified channel
    void printDebug(void);         // print the status of all channels
    float setFrequency(int pin, uint32_t frequency = 1000);
    uint8_t setResolution(int pin, uint8_t resolution = 10);

  private:
    float duty2deg(int ch, uint32_t duty, float countPerUs);
    void ledc_attach_with_invert(int pin, int ch);
    void config_servo(int ch, int minUs, int maxUs, double speed = 0, double ke = 1.0);
    void wr_servo(int pin, float value, double speed, double ke);
    void wr_ch_pair(int ch, uint32_t frequency, uint8_t resolution);
    void wr_duty(int ch, uint32_t duty);
    void wr_freq_res(int ch, uint32_t frequency, uint8_t resolution);
    void wr_phase(int ch, uint32_t duty, uint32_t phase);
    void reset_fields(int ch);
    bool sync = false;
};

#endif
