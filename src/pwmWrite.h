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
      float frequency;
      uint32_t duty;
      uint8_t resolution;
      uint8_t mode;
      uint8_t timer;
      uint32_t phase;
      uint16_t servoMinUs;
      uint16_t servoDefUs;
      uint16_t servoMaxUs;
      uint32_t startMs;
    } config_t;

    config_t config[16] = {
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

    float write(uint8_t pin, uint32_t duty);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
    float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);

    float read(uint8_t pin);
    float readMicroseconds(uint8_t pin);
    uint32_t writeServo(uint8_t pin, float value);
    uint8_t tone(uint8_t pin, uint32_t frequency, uint16_t duration = 0, uint16_t interval = 0);
    uint8_t note(uint8_t pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval);

    uint8_t attach(uint8_t pin);
    uint8_t attach(uint8_t pin, uint8_t ch);
    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    uint8_t attached(uint8_t pin);
    uint8_t attachedPin(uint8_t ch);
    void detach(uint8_t pin);

    void pause(uint8_t ch = 255);
    void resume(uint8_t ch = 255);

    float setFrequency(uint8_t pin, uint32_t frequency = 1000);
    uint8_t setResolution(uint8_t pin, uint8_t resolution = 10);
    void printConfig(void);

  private:

    enum State { ready, play, stop };
    State state = ready;

    void resetFields(uint8_t ch);
    void configServo(uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs);
    void writerFreqResPair(uint8_t ch, uint32_t frequency, uint8_t bits);
    bool _sync = false;
};
#endif
