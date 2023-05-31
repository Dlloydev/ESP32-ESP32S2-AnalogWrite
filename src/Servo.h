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

    uint8_t attach(int pin) {
      return pwm.attachServo(pin);
    };

    uint8_t attach(int pin, bool invert) {
      return pwm.attachServo(pin, invert);
    };

    uint8_t attach(int pin, int ch) {
      return pwm.attachServo(pin, ch);
    };

    uint8_t attach(int pin, int ch, bool invert) {
      return pwm.attachServo(pin, ch, invert);
    };

    uint8_t attach(int pin, int minUs, int maxUs) {
      return pwm.attachServo(pin, minUs, maxUs);
    };

    uint8_t attach(int pin, int ch, int minUs, int maxUs) {
      return pwm.attachServo(pin, ch, minUs, maxUs);
    };

    uint8_t attach(int pin, int ch, int minUs, int maxUs, bool invert) {
      return pwm.attachServo(pin, ch, minUs, maxUs, invert);
    };

    uint8_t attach(int pin, int minUs, int maxUs, double speed, double ke) {
      return pwm.attachServo(pin, minUs, maxUs, speed, ke);
    };

    uint8_t attach(int pin, int ch, int minUs, int maxUs, double speed, double ke) {
      return pwm.attachServo(pin, ch, minUs, maxUs, speed, ke);
    };

    uint8_t attach(int pin, int ch, int minUs, int maxUs, double speed, double ke, bool invert) {
      return pwm.attachServo(pin, ch, minUs, maxUs, speed, ke, invert);
    };

    float read(int pin) {
      return pwm.read(pin);
    };

    float readMicroseconds(int pin) {
      return pwm.readMicroseconds(pin);
    };

    float write(int pin, float value) {
      return pwm.writeServo(pin, value);
    };

    float writeMicroseconds(int pin, float value) {
      return pwm.writeServo(pin, value);
    };

    float write(int pin, float value, double speed, double ke) {
      return pwm.writeServo(pin, value, speed, ke);
    };

    float writeMicroseconds(int pin, float value, double speed, double ke) {
      return pwm.writeServo(pin, value, speed, ke);
    };

    // common

    uint8_t attached(int pin) {    // check if pin is attached
      return pwm.attached(pin);
    };

    uint8_t attachedPin(int ch) {  // get pin on specified channel
      return pwm.attachedPin(ch);
    };

    uint8_t firstFreeCh(void) {    // get first free channel
      return pwm.firstFreeCh();
    };

    void detach(int pin) {         // detach pin
      pwm.detach(pin);
    };

    bool detached(int pin) {       // check if pin is detached
      return pwm.detached(pin);
    };

    void pause(int ch = 255) {     // pause timer on all or specified channel
      pwm.pause(ch);
    };

    void resume(int ch = 255) {    // resume timer on all or specified channel
      pwm.resume(ch);
    };

    void printDebug(void) {        // print the status of all channels
      pwm.printDebug();
    };

    float setFrequency(int pin, uint32_t frequency = 1000) {
      return pwm.setFrequency(pin, frequency);
    };

    uint8_t setResolution(int pin, uint8_t resolution = 10) {
      return pwm.setResolution(pin, resolution);
    };

    // pwm

    float writePwm(int pin, uint32_t duty) {
      return pwm.write(pin, duty);
    };

    float writePwm(int pin, uint32_t duty, uint32_t frequency) {
      return pwm.write(pin, duty, frequency);
    };

    float writePwm(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution) {
      return pwm.write(pin, duty, frequency, resolution);
    };

    float writePwm(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
      return pwm.write(pin, duty, frequency, resolution, phase);
    };

    uint8_t attachPwm(int pin) {  // attach pin to next free channel
      return pwm.attach(pin);
    };

    uint8_t attachPwm(int pin, int ch) {  // attach to specified channel
      return pwm.attach(pin, ch);
    };

    uint8_t attachInvert(int pin) {  // attach pin to next free channel with inverted pwm
      return pwm.attachInvert(pin);
    };

    uint8_t attachInvert(int pin, int ch) {  // attach to specified ch with inverted pwm
      return pwm.attachInvert(pin, ch);
    };

    // tone and note

    void tone(int pin, uint32_t frequency, uint16_t duration = 500, uint16_t interval = 0) {
      pwm.tone(pin, frequency, duration, interval);
    };

    void note(int pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval) {
      pwm.note(pin, note, octave, duration, interval);
    };

};

#endif
