/*
   ESP32 PWM, SERVO and TONE Library, Version 5.0.2
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   License: MIT
*/

#include <Arduino.h>
#include "pwmWrite.h"

Pwm::Pwm() {}

float Pwm::write(int pin, uint32_t duty) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_duty(ch, duty);
  }
  return mem[ch].frequency;
}

float Pwm::write(int pin, uint32_t duty, uint32_t frequency) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, mem[ch].resolution);
    wr_duty(ch, duty);
  }
  return mem[ch].frequency;
}

float Pwm::write(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, resolution);
    wr_duty(ch, duty);
  }
  return mem[ch].frequency;
}

float Pwm::write(int pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, resolution);
    wr_phase(ch, duty, phase);
    wr_duty(ch, duty);
  }
  return mem[ch].frequency;
}

uint8_t Pwm::attach(int pin) {
  uint8_t ch = attached(pin);
  if (ch == 253) { // free channels exist
    for (uint8_t c = 0; c < chMax; c++) {
      if (mem[c].pin == 255 && ch == 253) { //first free ch
        mem[c].pin = pin;
        ch = c;
        ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
        if (sync) pause(ch);
        ledcAttachPin(pin, ch);
        return ch;
      }
    }
  }
  return ch;
}

uint8_t Pwm::attach(int pin, int ch) {
  if (mem[ch].pin == 255) {
    mem[ch].pin = pin;
    ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
    if (sync) pause(ch);
    ledcAttachPin(pin, ch);
  }
  return ch;
}

uint8_t Pwm::attachInvert(int pin) {
  uint8_t ch = firstFreeCh();
  if (ch < chMax) mem[ch].pin = pin;
  ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
  if (sync) pause(ch);
  ledc_attach_with_invert(pin, ch);
  return ch;
}

uint8_t Pwm::attachInvert(int pin, int ch) {
  if (mem[ch].pin == 255) {
    mem[ch].pin = pin;
    ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
    if (sync) pause(ch);
    ledc_attach_with_invert(pin, ch);
  }
  return ch;
}

uint8_t Pwm::attachServo(int pin) {
  uint8_t ch = firstFreeCh();
  if (ch < chMax) config_servo(ch, mem[ch].servoMinUs, mem[ch].servoMaxUs);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, bool invert) {
  uint8_t ch = firstFreeCh();
  if (ch < chMax) config_servo(ch, mem[ch].servoMinUs, mem[ch].servoMaxUs);
  return (invert) ? attachInvert(pin, ch) : attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch) {
  if (ch < chMax) config_servo(ch, mem[ch].servoMinUs, mem[ch].servoMaxUs);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch, bool invert) {
  if (ch < chMax) config_servo(ch, mem[ch].servoMinUs, mem[ch].servoMaxUs);
  return (invert) ? attachInvert(pin, ch) : attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int minUs, int maxUs) {
  uint8_t ch = firstFreeCh();
  if (ch < chMax) config_servo(ch, minUs, maxUs);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch, int minUs, int maxUs) {
  config_servo(ch, minUs, maxUs);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch, int minUs, int maxUs, bool invert) {
  config_servo(ch, minUs, maxUs);
  return (invert) ? attachInvert(pin, ch) : attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int minUs, int maxUs, double speed, double ke) {
  uint8_t ch = firstFreeCh();
  if (ch < chMax) config_servo(ch, minUs, maxUs, speed, ke);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch, int minUs, int maxUs, double speed, double ke) {
  config_servo(ch, minUs, maxUs, speed, ke);
  return attach(pin, ch);
}

uint8_t Pwm::attachServo(int pin, int ch, int minUs, int maxUs, double speed, double ke, bool invert) {
  config_servo(ch, minUs, maxUs, speed, ke);
  return (invert) ? attachInvert(pin, ch) : attach(pin, ch);
}

float Pwm::read(int pin) {
  uint8_t ch = attached(pin);
  if (ch < chMax) {
    float deg = (readMicroseconds(pin) - mem[ch].servoMinUs) / (mem[ch].servoMaxUs - mem[ch].servoMinUs) * 180.0;
    return deg < 0 ? 0 : deg;
  }
  else return 0;
}

float Pwm::readMicroseconds(int pin) {
  uint8_t ch = attached(pin);
  if (ch < chMax) return mem[ch].duty * ((1000000.0 / mem[ch].frequency) / ((1 << mem[ch].resolution) - 1.0)); // μs
  else return 0;
}

float Pwm::writeServo(int pin, float value, double speed, double ke) {
  uint8_t ch = attached(pin);
  wr_servo(pin, value, speed, ke);
  return mem[ch].ye; // normalized easing position (0.0 - 1.0)
}

float Pwm::writeServo(int pin, float value) {
  uint8_t ch = attached(pin);
  if (ch == 253) { // free channels exist
    for (uint8_t c = 0; c < chMax; c++) {
      if (mem[c].pin == 255 && ch == 253) { //first free ch
        mem[c].pin = pin;
        ch = c;
        if (mem[ch].frequency < 40 || mem[ch].frequency > 900) mem[ch].frequency = 50;
        if (mem[ch].resolution > widthMax) mem[ch].resolution = widthMax;
        else if (mem[ch].resolution < 14 && widthMax == 20) mem[ch].resolution = 16;
        else if (mem[ch].resolution < 14) mem[ch].resolution = 14;
        ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
        if (sync) pause(ch);
        ledcAttachPin(pin, ch);
      }
    }
  }
  wr_servo(pin, value, mem[ch].speed, mem[ch].ke);
  return mem[ch].ye; // normalized easing position (0.0 - 1.0)
}

void Pwm::tone(int pin, uint32_t frequency, uint16_t duration, uint16_t interval) {
  uint8_t ch = attach(pin);
  if (ch < chMax) {
    uint32_t ms = millis();
    static bool durDone = false;
#if defined(CONFIG_IDF_TARGET_ESP32C3)
    if (frequency < 153) frequency = 153;
#else
    if (frequency < 4) frequency = 4;
#endif
    if (!durDone) {
      if (frequency != mem[ch].frequency && (ms - mem[ch].startMs > interval)) {
        mem[ch].startMs = ms;
        mem[ch].frequency = frequency;
        ledcChangeFrequency(ch, frequency, mem[ch].resolution);
        write(pin, 127, frequency, 8);
        resume(ch);
      }
      if ((duration && ((ms - mem[ch].startMs) > duration)) || (duration == 0)) {
        mem[ch].startMs = ms;
        durDone = true;
        if (duration < 0xffff) pause(ch);
      }
    } else if (ms - mem[ch].startMs > interval) durDone = false;
  }
}

void Pwm::note(int pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval) {
  const uint16_t noteFrequencyBase[12] = {
    // C       C#        D       Eb        E        F       F#        G       G#        A       Bb        B
    4186,    4435,    4699,    4978,    5274,    5588,    5920,    6272,    6645,    7040,    7459,    7902
  };
  uint32_t noteFreq =  (uint32_t)noteFrequencyBase[note] / (uint32_t)(1 << (8 - octave));
  if (octave <= 8 || note <= NOTE_MAX) tone(pin, noteFreq, duration, interval);
}

uint8_t Pwm::attached(int pin) {
  if (!((pinMask >> pin) & 1)) return 254; // not a pwm pin
  bool freeCh = false;
  for (uint8_t ch = 0; ch < chMax; ch++) {
    if (mem[ch].pin == pin) return ch;
    if (mem[ch].pin == 255) freeCh = true; // free channel(s) exist
  }
  return (freeCh) ? 253 : 255; // freeCh : not attached
}

uint8_t Pwm::attachedPin(int ch) {
  return mem[ch].pin;
}

uint8_t Pwm::firstFreeCh(void) {
  for (uint8_t ch = 0; ch < chMax; ch++) {
    if (mem[ch].pin == 255) return ch;
  }
  return 255;
}

void Pwm::detach(int pin) {
  uint8_t ch = attached(pin);
  if (ch < chMax) {
    reset_fields(ch);
    if (digitalRead(pin) == HIGH) delayMicroseconds(mem[ch].servoMaxUs); // wait until LOW
    ledcWrite(ch, 4); // set minimal duty
    ledcDetachPin(mem[ch].pin); // jitterless
    REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
  }
}

bool Pwm::detached(int pin) {
  if ((REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) == 0) return true;
  else return false;
}

void Pwm::pause(int ch) {
  if (ch == 255) sync = true;
  else ledc_timer_pause((ledc_mode_t)mem[ch].mode, (ledc_timer_t)mem[ch].timer);
}

void Pwm::resume(int ch) {
  if (sync && ch == 255) {
    for (uint8_t ch = 0; ch < chMax; ch++) {
      if (mem[ch].pin < 48) ledc_timer_resume((ledc_mode_t)mem[ch].mode, (ledc_timer_t)mem[ch].timer);
    }
    sync = false;
  } else {
    ledc_timer_resume((ledc_mode_t)mem[ch].mode, (ledc_timer_t)mem[ch].timer);
  }
}

float Pwm::setFrequency(int pin, uint32_t frequency) {
  uint8_t ch = attach(pin);
  if (ch < chMax) {
    if (mem[ch].frequency != frequency) {
      ledcSetup(ch, frequency, mem[ch].resolution);
      if (sync) pause(ch);
      ledcWrite(ch, mem[ch].duty);
      mem[ch].frequency = frequency;
      wr_ch_pair(ch, frequency, mem[ch].resolution);
    }
  }
  return ledcReadFreq(ch);
}

uint8_t Pwm::setResolution(int pin, uint8_t resolution) {
  uint8_t ch = attach(pin);
  if (ch < chMax) {
    if (mem[ch].resolution != resolution) {
      ledcSetup(ch, mem[ch].frequency, resolution);
      if (sync) pause(ch);
      ledcWrite(ch, mem[ch].duty);
      mem[ch].resolution = resolution;
      wr_ch_pair(ch, mem[ch].frequency, resolution);
    }
  }
  return mem[ch].resolution;
}

void Pwm::printDebug() {
  Serial.printf("PWM pins:\n");
  for (uint8_t i = 0; i < 48; i++) {
    if ((pinMask >> i) & 1) {
      Serial.printf("%d,", i);
    }
  }
  Serial.printf("\n\nCh  Pin     Hz Res  Duty  Servo     Speed   ke\n");
  for (uint8_t ch = 0; ch < chMax; ch++) {
    Serial.printf ("%2d  %3d  %5.0f  %2d  %4d  %d-%d  %5.1f  %1.1f\n", ch, mem[ch].pin, mem[ch].frequency, mem[ch].resolution,
                   mem[ch].duty, mem[ch].servoMinUs, mem[ch].servoMaxUs, mem[ch].speed, mem[ch].ke);
  }
  Serial.printf("\n");
}

/***** private functions *****/

void Pwm::ledc_attach_with_invert(int pin, int ch) {
  if (ch >= chMax) return;
  uint32_t ch_config = ch;
  if (ch > 7) ch_config = ch - 8;
  ledc_channel_config_t ledc_channel = {
    .gpio_num       = (mem[ch].pin),
    .speed_mode     = (ledc_mode_t) mem[ch].mode,
    .channel        = (ledc_channel_t) ch_config,
    .intr_type      = (ledc_intr_type_t) LEDC_INTR_DISABLE,
    .timer_sel      = (ledc_timer_t) mem[ch].timer,
    .duty           = 0,
    .hpoint         = 0,
    .flags = {
      .output_invert = 1
    }
  };
  ledc_channel_config(&ledc_channel);
}

float Pwm::duty2deg(int ch, uint32_t duty, float countPerUs) {
  return ((duty / countPerUs - mem[ch].servoMinUs) / (mem[ch].servoMaxUs - mem[ch].servoMinUs)) * 180.0;
}

void Pwm::config_servo(int ch, int minUs, int maxUs, double speed, double ke) {
  if (minUs < 500) mem[ch].servoMinUs = 500;
  else if (minUs > 2500) mem[ch].servoMinUs = 2500;
  else mem[ch].servoMinUs = minUs;
  if (maxUs < 500) mem[ch].servoMaxUs = 500;
  else if (maxUs > 2500) mem[ch].servoMaxUs = 2500;
  else mem[ch].servoMaxUs = maxUs;
  if (mem[ch].frequency < 40 || mem[ch].frequency > 900) mem[ch].frequency = 50;
  if (mem[ch].resolution > widthMax) mem[ch].resolution = widthMax;
  else if (mem[ch].resolution < 14 && widthMax == 20) mem[ch].resolution = 16;
  else if (mem[ch].resolution < 14) mem[ch].resolution = 14;
  speed = (speed > 2000) ? 2000 : (speed < 0) ? 0 : speed;
  mem[ch].speed = speed;
  ke = (ke > 1.0) ? 1.0 : (ke < 0) ? 0 : ke;
  mem[ch].ke = ke;
  wr_ch_pair(ch, mem[ch].frequency, mem[ch].resolution);
}

void Pwm::wr_servo(int pin, float value, double speed, double ke) {
  uint8_t ch = attached(pin);
  if (ch == 253) { // free channels exist
    for (uint8_t c = 0; c < chMax; c++) {
      if (mem[c].pin == 255 && ch == 253) { //first free ch
        mem[c].pin = pin;
        ch = c;
        if (mem[ch].frequency < 40 || mem[ch].frequency > 900) mem[ch].frequency = 50;
        if (mem[ch].resolution > widthMax) mem[ch].resolution = widthMax;
        else if (mem[ch].resolution < 14 && widthMax == 20) mem[ch].resolution = 16;
        else if (mem[ch].resolution < 14) mem[ch].resolution = 14;
        ledcSetup(ch, mem[ch].frequency, mem[ch].resolution);
        if (sync) pause(ch);
        ledcAttachPin(pin, ch);
      }
    }
  }
  uint32_t duty;
  if (ch < chMax) { // write PWM
    float countPerUs = ((1 << mem[ch].resolution) - 1) / (1000000.0 / mem[ch].frequency);
    if (value < mem[ch].servoMinUs) {  // degrees
      if (value < 0) value = 0;
      else if (value > 180 && value < 500) value = 180;
      duty = (((value / 180.0) * (mem[ch].servoMaxUs - mem[ch].servoMinUs)) + mem[ch].servoMinUs) * countPerUs;
    } else {  // microseconds
      if (value < mem[ch].servoMinUs) value = mem[ch].servoMinUs;
      else if (value > mem[ch].servoMaxUs) value = mem[ch].servoMaxUs;
      duty = value * countPerUs;
    }
    ke = (ke > 1.0) ? 1.0 : (ke < 0) ? 0 : ke;
    if (ke < 1.0) { // easing enabled
      float deltaDeg;
      uint32_t easeDuty;
      if (ke < 1.0 && duty != mem[ch].duty) { // init
        mem[ch].startDuty = mem[ch].stopDuty;
        mem[ch].stopDuty = duty;
        mem[ch].deltaDuty = (mem[ch].startDuty < mem[ch].stopDuty) ? mem[ch].stopDuty - mem[ch].startDuty : mem[ch].startDuty - mem[ch].stopDuty;
        mem[ch].startMs = millis();
        speed = (speed > 2000) ? 2000 : (speed < 0) ? 0 : speed;
        if (speed > 0) {
          deltaDeg = fabsf(duty2deg(ch, mem[ch].stopDuty, countPerUs) - duty2deg(ch, mem[ch].startDuty, countPerUs));
          mem[ch].stopMs = mem[ch].startMs + (deltaDeg / speed) * 1000;
          mem[ch].deltaMs = mem[ch].stopMs - mem[ch].startMs;
        }
      }
      uint32_t now = millis();
      if (duty > mem[ch].startDuty) {
        mem[ch].te = (float)(now - mem[ch].startMs) / mem[ch].deltaMs;
        mem[ch].ye = (-(ke + 1.0f) * (2.0f * mem[ch].te - 1.0f) / (2.0f * (-4.0f * ke * fabsf(mem[ch].te - 0.5f) + ke - 1.0f))) + 0.5f;
        if (isnan(mem[ch].ye) || mem[ch].ye < 0.0) mem[ch].ye = 0.0;
        else if (mem[ch].ye > 1.0) mem[ch].ye = 1.0;
        easeDuty = mem[ch].startDuty + (mem[ch].deltaDuty * mem[ch].ye);
      } else {
        mem[ch].te = 1 - ((float)(now - mem[ch].startMs) / mem[ch].deltaMs);
        mem[ch].ye = 1 - (((ke + 1.0f) * (2.0f * mem[ch].te - 1.0f) / (2.0f * (-4.0f * ke * fabsf(mem[ch].te - 0.5f) + ke - 1.0f))) + 0.5f);
        if (isnan(mem[ch].ye) || mem[ch].ye < 0.0) mem[ch].ye = 0.0;
        else if (mem[ch].ye > 1.0) mem[ch].ye = 1.0;
        easeDuty = duty + (mem[ch].deltaDuty * mem[ch].ye);
      }
      ledcWrite(ch, easeDuty);
    } else ledcWrite(ch, duty);
    mem[ch].duty = duty;
  }
}

void Pwm::wr_ch_pair(int ch, uint32_t frequency, uint8_t bits) {
  mem[ch].frequency = frequency;
  mem[ch].resolution = bits;
  if (ch % 2 == 0) { // even ch
    mem[ch + 1].frequency = frequency;
    mem[ch + 1].resolution = bits;
  } else { // odd ch
    mem[ch - 1].frequency = frequency;
    mem[ch - 1].resolution = bits;
  }
}

void Pwm::wr_duty(int ch, uint32_t duty) {
  if (mem[ch].duty != duty) {
    ledcWrite(ch, duty);
    mem[ch].duty = duty;
  }
}

void Pwm::wr_freq_res(int ch, uint32_t frequency, uint8_t resolution) {
  if ((mem[ch].frequency != frequency) || (mem[ch].resolution != resolution)) {
    ledcSetup(ch, frequency, resolution);
    wr_ch_pair(ch, frequency, resolution);
    mem[ch].frequency = frequency;
    mem[ch].resolution = resolution;
  }
}

void Pwm::wr_phase(int ch, uint32_t duty, uint32_t phase) {
  if (mem[ch].phase != phase) {
    uint32_t ch_config = ch;
    if (ch > 7) ch_config = ch - 8;
    ledc_set_duty_with_hpoint((ledc_mode_t)mem[ch].mode, (ledc_channel_t)ch_config, duty, phase);
    if (sync) pause(ch);
    mem[ch].phase = phase;
  }
}

void Pwm::reset_fields(int ch) {
  mem[ch].pin = 255;
  mem[ch].duty = 0;
  mem[ch].frequency = 1000;
  mem[ch].resolution = 8;
  mem[ch].phase = 0;
  mem[ch].servoMinUs = 544;
  mem[ch].servoMaxUs = 2400;
  mem[ch].speed = 0;
  mem[ch].ke = 1.0;
}
