#pragma once
#ifndef Servo_h
#define Servo_h

#include <inttypes.h>
#include "pwmWrite.h"

class Servo
{

    Pwm pwm;

  private:

  public:

    // servo

    uint8_t attach(uint8_t pin) {
      return pwm.attachServo(pin);
    };

    uint8_t attach(uint8_t pin, bool invert) {
      return pwm.attachServo(pin, invert);
    };

    uint8_t attach(uint8_t pin, uint8_t ch) {
      return pwm.attachServo(pin, ch);
    };

    uint8_t attach(uint8_t pin, uint8_t ch, bool invert) {
      return pwm.attachServo(pin, ch, invert);
    };

    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t maxUs) {
      return pwm.attachServo(pin, minUs, maxUs);
    };

    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs) {
      return pwm.attachServo(pin, ch, minUs, maxUs);
    };

    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs, bool invert) {
      return pwm.attachServo(pin, ch, minUs, maxUs, invert);
    };

    uint8_t attach(uint8_t pin, uint16_t minUs, uint16_t maxUs, float speed, float ke) {
      return pwm.attachServo(pin, minUs, maxUs, speed, ke);
    };

    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs, float speed, float ke) {
      return pwm.attachServo(pin, ch, minUs, maxUs, speed, ke);
    };

    uint8_t attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t maxUs, float speed, float ke, bool invert) {
      return pwm.attachServo(pin, ch, minUs, maxUs, speed, ke, invert);
    };

    float read(uint8_t pin) {
      return pwm.read(pin);
    };

    float readMicroseconds(uint8_t pin) {
      return pwm.readMicroseconds(pin);
    };

    float write(uint8_t pin, float value) {
      return pwm.writeServo(pin, value);
    };

    float writeMicroseconds(uint8_t pin, float value) {
      return pwm.writeServo(pin, value);
    };

    float write(uint8_t pin, float value, float speed, float ke) {
      return pwm.writeServo(pin, value, speed, ke);
    };

    float writeMicroseconds(uint8_t pin, float value, float speed, float ke) {
      return pwm.writeServo(pin, value, speed, ke);
    };

    // common

    uint8_t attached(uint8_t pin) {    // check if pin is attached
      return pwm.attached(pin);
    };

    uint8_t attachedPin(uint8_t ch) {  // get pin on specified channel
      return pwm.attachedPin(ch);
    };

    uint8_t firstFreeCh(void) {        // get first free channel
      return pwm.firstFreeCh();
    };

    void detach(uint8_t pin) {         // detach pin
      pwm.detach(pin);
    };

    bool detached(uint8_t pin) {       // check if pin is detached
      return pwm.detached(pin);
    };

    void pause(uint8_t ch = 255) {     // pause timer on all or specified channel
      pwm.pause(ch);
    };

    void resume(uint8_t ch = 255) {    // resume timer on all or specified channel
      pwm.resume(ch);
    };

    void printDebug(void) {            // print the status of all channels
      pwm.printDebug();
    };

    float setFrequency(uint8_t pin, uint32_t frequency = 1000) {
      return pwm.setFrequency(pin, frequency);
    };

    uint8_t setResolution(uint8_t pin, uint8_t resolution = 10) {
      return pwm.setResolution(pin, resolution);
    };

    // pwm

    float writePwm(uint8_t pin, uint32_t duty) {
      return pwm.write(pin, duty);
    };

    float writePwm(uint8_t pin, uint32_t duty, uint32_t frequency) {
      return pwm.write(pin, duty, frequency);
    };

    float writePwm(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution) {
      return pwm.write(pin, duty, frequency, resolution);
    };

    float writePwm(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
      return pwm.write(pin, duty, frequency, resolution, phase);
    };

    uint8_t attachPwm(uint8_t pin) {  // attach pin to next free channel
      return pwm.attach(pin);
    };

    uint8_t attachPwm(uint8_t pin, uint8_t ch) {  // attach to specified channel
      return pwm.attach(pin, ch);
    };

    uint8_t attachInvert(uint8_t pin) {  // attach pin to next free channel with inverted pwm
      return pwm.attachInvert(pin);
    };

    uint8_t attachInvert(uint8_t pin, uint8_t ch) {  // attach to specified ch with inverted pwm
      return pwm.attachInvert(pin, ch);
    };

    // tone and note

    void tone(uint8_t pin, uint32_t frequency, uint16_t duration = 500, uint16_t interval = 0) {
      pwm.tone(pin, frequency, duration, interval);
    };

    void note(uint8_t pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval) {
      pwm.note(pin, note, octave, duration, interval);
    };

};

#endif
