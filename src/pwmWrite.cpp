/*******************************************************************
   ESP32 PWM, SERVO and TONE Library, Version 4.2.4
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 *******************************************************************/

#include <Arduino.h>
#include "pwmWrite.h"

Pwm::Pwm() {}

float Pwm::write(uint8_t pin, uint32_t duty) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_duty(ch, duty);
  }
  return config[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, config[ch].resolution);
    wr_duty(ch, duty);
  }
  return config[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, resolution);
    wr_duty(ch, duty);
  }
  return config[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
  uint8_t ch = attach(pin);
  if (ch < chMax) { // write PWM
    wr_freq_res(ch, frequency, resolution);
    wr_phase(ch, duty, phase);
    wr_duty(ch, duty);
  }
  return config[ch].frequency;
}

uint32_t Pwm::writeServo(uint8_t pin, float value) {
  uint8_t ch = attach(pin);
  float countPerUs;
  uint32_t duty = config[ch].servoDefUs;
  if (ch < chMax) { // write PWM
    if (config[ch].frequency < 40 || config[ch].frequency > 900) config[ch].frequency = 50;
    if (config[ch].resolution > widthMax) config[ch].resolution = widthMax;
    else if (config[ch].resolution < 14 && widthMax == 20) config[ch].resolution = 16;
    else if (config[ch].resolution < 14) config[ch].resolution = 14;
    countPerUs = ((1 << config[ch].resolution) - 1) / (1000000.0 / config[ch].frequency);
    if (value < config[ch].servoMinUs) {  // degrees
      if (value < 0) value = 0;
      else if (value > 180 && value < 500) value = 180;
      duty = (((value / 180.0) * (config[ch].servoMaxUs - config[ch].servoMinUs)) + config[ch].servoMinUs) * countPerUs;
    } else {  // microseconds
      if (value < config[ch].servoMinUs) value = config[ch].servoMinUs;
      else if (value > config[ch].servoMaxUs) value = config[ch].servoMaxUs;
      duty = value * countPerUs;
    }
    wr_duty(ch, duty);
  }
  return duty;
}

uint8_t Pwm::tone(uint8_t pin, uint32_t frequency, uint16_t duration, uint16_t interval) {
  uint8_t ch = attached(pin);
  if (ch == 253) {
    ch = attach(pin);
    write(pin, 127, frequency, 8);
    pause(ch);
  }
  switch (state) {
    case ready:
      if (duration) {
        config[ch].startMs = millis();
        (duration < 0xffff) ? pause(ch) : resume(ch);
        state = play;
      }
      return 0;
      break;
    case play:
      if (frequency != config[ch].frequency) {
        config[ch].startMs = millis();
        ledcChangeFrequency(ch, frequency, config[ch].resolution);
        config[ch].frequency = frequency;
        resume(ch);
      }
      if (millis() - config[ch].startMs >= duration) {
        config[ch].startMs = millis();
        (duration < 0xffff) ? pause(ch) : resume(ch);
        state = stop;
        return 2;
      }
      return 1;
      break;
    case stop:
      if (millis() - config[ch].startMs >= interval) {
        state = ready;
        return 0;
      }
      return 2;
      break;
  }
  return 0;
}

uint8_t Pwm::note(uint8_t pin, note_t note, uint8_t octave, uint16_t duration, uint16_t interval) {
  const uint16_t noteFrequencyBase[12] = {
    // C       C#        D       Eb        E        F       F#        G       G#        A       Bb        B
    4186,    4435,    4699,    4978,    5274,    5588,    5920,    6272,    6645,    7040,    7459,    7902
  };

  if (octave > 8 || note >= NOTE_MAX) {
    return 0;
  }
  uint32_t noteFreq =  (uint32_t)noteFrequencyBase[note] / (uint32_t)(1 << (8 - octave));
  return tone(pin, noteFreq, duration, interval);
}

float Pwm::read(uint8_t pin) {
  uint8_t ch = attached(pin);
  float deg;
  if (ch < chMax) {
    deg = (readMicroseconds(pin) - config[ch].servoMinUs) / (config[ch].servoMaxUs - config[ch].servoMinUs) * 180.0;
    if (deg < 0) deg = 0;
    return deg;
  }
  else return 0;
}

float Pwm::readMicroseconds(uint8_t pin) {
  uint8_t ch = attached(pin);
  if (ch < chMax) return config[ch].duty * ((1000000.0 / config[ch].frequency) / ((1 << config[ch].resolution) - 1.0)); // μs
  else return 0;
}

uint8_t Pwm::attach(uint8_t pin) {
  uint8_t chan = attached(pin);
  if (chan == 253) { // pin is free
    for (uint8_t ch = 0; ch < chMax; ch++) {
      if (config[ch].pin == 255) { // ch is free
        config[ch].pin = pin;
        ledcSetup(ch, config[ch].frequency, config[ch].resolution);
        if (sync) pause(ch);
        ledcAttachPin(pin, ch);
        return ch;
      }
    }
  }
  return chan;
}

uint8_t Pwm::attach(uint8_t pin, uint16_t minUs, uint16_t defUs, uint16_t maxUs) {
  uint8_t ch = attach(pin);
  if (ch < chMax) config_servo(ch, minUs, defUs, maxUs);
  return ch;
}

uint8_t Pwm::attach(uint8_t pin, uint8_t ch, bool invert) {
  uint8_t chan = attached(pin);
  if (config[ch].pin == 255) { // ch is free
    config[ch].pin = pin;
    ledcSetup(ch, config[ch].frequency, config[ch].resolution);
    if (sync) pause(ch);
    ledc_attach_with_invert(pin, ch, invert);
    return ch;
  }
  return chan;
}

uint8_t Pwm::attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs) {
  config_servo(ch, minUs, defUs, maxUs);
  return attach(pin, ch);
}

uint8_t Pwm::attach(uint8_t pin, uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs, bool invert) {
  config_servo(ch, minUs, defUs, maxUs);
  return attach(pin, ch, invert);
}

uint8_t Pwm::attached(uint8_t pin) {
  if (!((pinMask >> pin) & 1)) return 254; // not a pwm pin
  else {
    uint8_t free = 0;
    for (uint8_t ch = 0; ch < chMax; ch++) {
      if (config[ch].pin == pin) return ch;
      if (config[ch].pin == 255) free++;
    }
    if (free) return 253; // free channel(s) exist
  }
  return 255;  // not attached
}

uint8_t Pwm::attachedPin(uint8_t ch) {
  return config[ch].pin;
}

void Pwm::detach(uint8_t pin) {
  uint8_t ch = attached(pin);
  if (ch < chMax) {
    reset_fields(ch);
    ledcDetachPin(config[ch].pin);
    REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
  }
}

bool Pwm::detached(uint8_t pin) {
  if ((REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) == 0) return true;
  else return false;
}

void Pwm::pause(uint8_t ch) {
  if (ch == 255) sync = true;
  else ledc_timer_pause((ledc_mode_t)config[ch].mode, (ledc_timer_t)config[ch].timer);
}

void Pwm::resume(uint8_t ch) {
  if (sync && ch == 255) {
    for (uint8_t ch = 0; ch < chMax; ch++) {
      if (config[ch].pin < 48) ledc_timer_resume((ledc_mode_t)config[ch].mode, (ledc_timer_t)config[ch].timer);
    }
    sync = false;
  } else {
    ledc_timer_resume((ledc_mode_t)config[ch].mode, (ledc_timer_t)config[ch].timer);
  }
}

float Pwm::setFrequency(uint8_t pin, uint32_t frequency) {
  uint8_t ch = attach(pin);
  if (ch < chMax) {
    if (config[ch].frequency != frequency) {
      ledcSetup(ch, frequency, config[ch].resolution);
      if (sync) pause(ch);
      ledcWrite(ch, config[ch].duty);
      config[ch].frequency = frequency;
      wr_ch_pair(ch, frequency, config[ch].resolution);
    }
  }
  return ledcReadFreq(ch);
}

uint8_t Pwm::setResolution(uint8_t pin, uint8_t resolution) {
  uint8_t ch = attach(pin);
  if (ch < chMax) {
    if (config[ch].resolution != resolution) {
      ledcSetup(ch, config[ch].frequency, resolution);
      if (sync) pause(ch);
      ledcWrite(ch, config[ch].duty);
      config[ch].resolution = resolution;
      wr_ch_pair(ch, config[ch].frequency, resolution);
    }
  }
  return config[ch].resolution;
}

void Pwm::printConfig() {
  Serial.print(F("PWM pins: "));
  for (uint8_t i = 0; i < 48; i++) {
    if ((pinMask >> i) & 1) {
      Serial.print(i); Serial.print(F(", "));
    }
  }
  Serial.println();
  Serial.println();
  Serial.println(F("Ch  Pin   Hz     Res  Duty Phase  Servo"));
  for (uint8_t ch = 0; ch < chMax; ch++) {
    if (ch < 10) Serial.print(F(" "));
    Serial.print(ch);
    Serial.print(F("  "));
    if (config[ch].pin < 100) Serial.print(F(" "));
    if (config[ch].pin < 10) Serial.print(F(" "));
    Serial.print(config[ch].pin); Serial.print(F("  "));
    if (config[ch].frequency < 10000) Serial.print(F(" "));
    if (config[ch].frequency < 1000) Serial.print(F(" "));
    if (config[ch].frequency < 100) Serial.print(F(" "));
    if (config[ch].frequency < 10) Serial.print(F(" "));
    Serial.print(config[ch].frequency, 1); Serial.print(F("  "));
    if (config[ch].resolution < 10) Serial.print(F(" "));
    Serial.print(config[ch].resolution); Serial.print(F("  "));
    if (config[ch].duty < 1000) Serial.print(F(" "));
    if (config[ch].duty < 100) Serial.print(F(" "));
    if (config[ch].duty < 10) Serial.print(F(" "));
    Serial.print(config[ch].duty); Serial.print(F("  "));
    if (config[ch].phase < 1000) Serial.print(F(" "));
    if (config[ch].phase < 100) Serial.print(F(" "));
    if (config[ch].phase < 10) Serial.print(F(" "));
    Serial.print(config[ch].phase); Serial.print(F("  "));
    Serial.print(config[ch].servoMinUs); Serial.print(F(" "));
    Serial.print(config[ch].servoDefUs); Serial.print(F(" "));
    Serial.print(config[ch].servoMaxUs);
    Serial.println();
  }
}

/************************* private functions ***************************/

void Pwm::ledc_attach_with_invert(uint8_t pin, uint8_t ch, bool invert) {
  if (ch >= chMax) return;
  uint32_t ch_config = ch;
  if (ch > 7) ch_config = ch - 8;
  ledc_channel_config_t ledc_channel = {
    .gpio_num       = (config[ch].pin),
    .speed_mode     = (ledc_mode_t) config[ch].mode,
    .channel        = (ledc_channel_t) ch_config,
    .intr_type      = (ledc_intr_type_t) LEDC_INTR_DISABLE,
    .timer_sel      = (ledc_timer_t) config[ch].timer,
    .duty           = 0,
    .hpoint         = 0,
    .flags = {
      .output_invert = (uint8_t)invert
    }
  };
  ledc_channel_config(&ledc_channel);
}

void Pwm::config_servo(uint8_t ch, uint16_t minUs, uint16_t defUs, uint16_t maxUs) {
  if (minUs < 500) config[ch].servoMinUs = 500;
  else if (minUs > 2500) config[ch].servoMinUs = 2500;
  else config[ch].servoMinUs = minUs;
  if (defUs < 500) config[ch].servoDefUs = 500;
  else if (defUs > 2500) config[ch].servoDefUs = 2500;
  else config[ch].servoDefUs = defUs;
  if (maxUs < 500) config[ch].servoMaxUs = 500;
  else if (maxUs > 2500) config[ch].servoMaxUs = 2500;
  else config[ch].servoMaxUs = maxUs;
  if (config[ch].frequency < 40 || config[ch].frequency > 900) config[ch].frequency = 50;
  if (config[ch].resolution > widthMax) config[ch].resolution = widthMax;
  else if (config[ch].resolution < 14 && widthMax == 20) config[ch].resolution = 16;
  else if (config[ch].resolution < 14) config[ch].resolution = 14;
  wr_ch_pair(ch, config[ch].frequency, config[ch].resolution);
}

void Pwm::wr_ch_pair(uint8_t ch, uint32_t frequency, uint8_t bits) {
  config[ch].frequency = frequency;
  config[ch].resolution = bits;
  if (ch % 2 == 0) { // even ch
    config[ch + 1].frequency = frequency;
    config[ch + 1].resolution = bits;
  } else { // odd ch
    config[ch - 1].frequency = frequency;
    config[ch - 1].resolution = bits;
  }
}

void Pwm::wr_duty(uint8_t ch, uint32_t duty) {
  if (config[ch].duty != duty) {
    ledcSetup(ch, config[ch].frequency, config[ch].resolution);
    ledcWrite(ch, duty);
    config[ch].duty = duty;
  }
}

void Pwm::wr_freq_res(uint8_t ch, uint32_t frequency, uint8_t resolution) {
  if ((config[ch].frequency != frequency) || (config[ch].resolution != resolution)) {
    ledcSetup(ch, frequency, resolution);
    wr_ch_pair(ch, frequency, resolution);
    config[ch].frequency = frequency;
    config[ch].resolution = resolution;
  }
}

void Pwm::wr_phase(uint8_t ch, uint32_t duty, uint32_t phase) {
  if (config[ch].phase != phase) {
    uint32_t ch_config = ch;
    if (ch > 7) ch_config = ch - 8;
    ledc_set_duty_with_hpoint((ledc_mode_t)config[ch].mode, (ledc_channel_t)ch_config, duty, phase);
    if (sync) pause(ch);
    config[ch].phase = phase;
  }
}

void Pwm::reset_fields(uint8_t ch) {
  config[ch].pin = 255;
  config[ch].duty = 0;
  config[ch].frequency = 1000;
  config[ch].resolution = 8;
  config[ch].phase = 0;
  config[ch].servoMinUs = 544;
  config[ch].servoDefUs = 1472;
  config[ch].servoMaxUs = 2400;
}
