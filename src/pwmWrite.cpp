/*******************************************************************
   pwmWrite Library for ESP32 Arduino core, Version 4.0.0
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 *******************************************************************/

#include <Arduino.h>
#include "pwmWrite.h"
#include <stdlib.h>

Pwm::Pwm() {}

void Pwm::pause() {
  _sync = true;
}

void Pwm::resume() {
  for (uint8_t i = 0; i < chMax; i++) {
    if (pinsStatus[i].pin < 48) timerResume(i);
  }
  _sync = false;
}

void Pwm::attachPin(uint8_t pin, uint8_t ch) {
  if (pin < 48 && ch < chMax) {
    pinsStatus[ch].pin = pin;
    ledcSetup(ch, pinsStatus[ch].frequency, pinsStatus[ch].resolution);
    if (_sync) timerPause(ch);
    ledcAttachPin(pin, ch);
  }
}

void Pwm::detachPin(uint8_t pin, uint8_t ch) {
  pinsStatus[ch].pin = 255;
  pinsStatus[ch].duty = 0;
  pinsStatus[ch].frequency = 1000;
  pinsStatus[ch].resolution = 8;
  pinsStatus[ch].phase = 0;
  ledcWrite(ch, 0);
  ledcSetup(ch, 0, 0);
  if (_sync) timerPause(ch);
  ledcDetachPin(pinsStatus[ch].pin);
  REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
}

void Pwm::timerPause(uint8_t ch) {
  ledc_timer_pause((ledc_mode_t)pinsStatus[ch].mode, (ledc_timer_t)pinsStatus[ch].timer);
}

void Pwm::timerResume(uint8_t ch) {
  ledc_timer_resume((ledc_mode_t)pinsStatus[ch].mode, (ledc_timer_t)pinsStatus[ch].timer);
}

uint8_t Pwm::getChannel(uint8_t pin) {
  if (!((pinMask >> pin) & 1)) return 254; //not pwm pin
  for (uint8_t i = 0; i < chMax; i++) {
    uint8_t ch = pinsStatus[i].channel;
    if (pinsStatus[ch].pin == pin) {
      return ch;
      break;
    }
  }
  for (uint8_t i = 0; i < chMax; i++) {
    uint8_t ch = pinsStatus[i].channel;
    if ((REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) == 0) { //free pin
      if (pinsStatus[ch].pin == 255) { //free channel
        pinsStatus[ch].pin = pin;
        ledcSetup(ch, pinsStatus[ch].frequency, pinsStatus[ch].resolution);
        if (_sync) timerPause(ch);
        ledcAttachPin(pin, ch);
        return ch;
        break;
      }
    } else {
      return 253; //occupied pin
      break;
    }
  }
  return 252; //error
}

void  Pwm::configChannel(uint8_t ch) {
  uint32_t ch_config = ch;
  if (ch > 7) ch_config = ch - 8;
  ledc_channel_config_t ledc_channel = {
    .gpio_num       = (pinsStatus[ch].pin),
    .speed_mode     = (ledc_mode_t) pinsStatus[ch].mode,
    .channel        = (ledc_channel_t) ch_config,
    .intr_type      = (ledc_intr_type_t) LEDC_INTR_DISABLE,
    .timer_sel      = (ledc_timer_t) pinsStatus[ch].timer,
    .duty           = 0,
    .hpoint         = 0,
    .flags = {
      .output_invert = 0
    }
  };
  ledc_channel_config(&ledc_channel);
}

float Pwm::setFrequency(uint8_t pin, uint32_t frequency) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) {
    if ((pinsStatus[ch].pin) > 47) return 255;
    if (pinsStatus[ch].frequency != frequency) {
      configChannel(ch);
      ledcSetup(ch, frequency, pinsStatus[ch].resolution);
      if (_sync) timerPause(ch);
      ledcWrite(ch, pinsStatus[ch].duty);
      pinsStatus[ch].frequency = frequency;
      if (ch % 2 == 0) { // even ch
        pinsStatus[ch + 1].frequency = frequency;
      } else { // odd ch
        pinsStatus[ch - 1].frequency = frequency;
      }
    }
  }
  return ledcReadFreq(ch);
}

uint32_t Pwm::setResolution(uint8_t pin, uint8_t resolution) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) {
    if ((pinsStatus[ch].pin) > 47) return 255;
    if (pinsStatus[ch].resolution != resolution) {
      ledcDetachPin(pin);
      configChannel(ch);
      ledcSetup(ch, pinsStatus[ch].frequency, resolution & 0xF);
      if (_sync) timerPause(ch);
      ledcAttachPin(pin, ch);
      ledcWrite(ch, pinsStatus[ch].duty);
      pinsStatus[ch].resolution = resolution & 0xF;
      if (ch % 2 == 0) { // even ch
        pinsStatus[ch + 1].resolution = resolution & 0xF;
      } else { // odd ch
        pinsStatus[ch - 1].resolution = resolution & 0xF;
      }
    }
  }
  return 1 << (resolution & 0xF);
}

void Pwm::setPinsStatusDefaults(uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
  for (uint8_t i = 0; i < chMax; i++) {
    pinsStatus[i].duty = duty;
    pinsStatus[i].frequency = frequency;
    pinsStatus[i].resolution = resolution;
    pinsStatus[i].phase = phase;
  }
}

void Pwm::printPinsStatus() {
  Serial.print(F("PWM pins: "));
  for (int i = 0; i < 48; i++) {
    if ((pinMask >> i) & 1) {
      Serial.print(i); Serial.print(F(", "));
    }
  }
  Serial.println();
  Serial.println();
  for (uint8_t i = 0; i < chMax; i++) {
    uint8_t ch = pinsStatus[i].channel;
    Serial.print(F("ch: "));
    if (ch < chMax) {
      if (pinsStatus[ch].channel < 10) Serial.print(F(" "));
      Serial.print(ch);
      Serial.print(F("  "));
      Serial.print(F("Pin: "));
      if (pinsStatus[ch].pin < 10) Serial.print(F(" "));
      Serial.print(pinsStatus[ch].pin); Serial.print(F("  "));
      Serial.print(F("Hz: "));
      if (pinsStatus[ch].frequency < 10000) Serial.print(F(" "));
      if (pinsStatus[ch].frequency < 1000) Serial.print(F(" "));
      if (pinsStatus[ch].frequency < 100) Serial.print(F(" "));
      if (pinsStatus[ch].frequency < 10) Serial.print(F(" "));
      Serial.print(pinsStatus[ch].frequency); Serial.print(F("  "));
      Serial.print(F("Bits: "));
      if (pinsStatus[ch].resolution < 10) Serial.print(F(" "));
      Serial.print(pinsStatus[ch].resolution); Serial.print(F("  "));
      Serial.print(F("Duty: "));
      if (pinsStatus[ch].duty < 10000) Serial.print(F(" "));
      if (pinsStatus[ch].duty < 1000) Serial.print(F(" "));
      if (pinsStatus[ch].duty < 100) Serial.print(F(" "));
      if (pinsStatus[ch].duty < 10) Serial.print(F(" "));
      Serial.print(pinsStatus[ch].duty); Serial.print(F("  "));
      Serial.print(F("Ø: "));
      if (pinsStatus[ch].phase < 1000) Serial.print(F(" "));
      if (pinsStatus[ch].phase < 100) Serial.print(F(" "));
      if (pinsStatus[ch].phase < 10) Serial.print(F(" "));
      Serial.print(pinsStatus[ch].phase);
      Serial.println();
    }
  }
}

float Pwm::write(uint8_t pin, uint32_t duty) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) { // write PWM
    if ((pinsStatus[ch].pin) > 47) return 255;
    uint8_t bits = pinsStatus[ch].resolution;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if (pinsStatus[ch].duty != duty) {
      configChannel(ch);
      ledcSetup(ch, pinsStatus[ch].frequency, bits);
      if (_sync) timerPause(ch);
      ledcWrite(ch, duty);
      pinsStatus[ch].duty = duty;
    }
  }
  return pinsStatus[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) { // write PWM
    uint8_t bits = pinsStatus[ch].resolution;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if (pinsStatus[ch].frequency != frequency) {
      configChannel(ch);
      ledcSetup(ch, frequency, bits);
      if (_sync) timerPause(ch);
      ledcWrite(ch, duty);
      pinsStatus[ch].frequency = frequency;
      if (ch % 2 == 0) { // even ch
        pinsStatus[ch + 1].frequency = frequency;
      } else { // odd ch
        pinsStatus[ch - 1].frequency = frequency;
      }
    }
    if (pinsStatus[ch].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch].duty = duty;
    }
  }
  return pinsStatus[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) { // write PWM
    if ((pinsStatus[ch].pin) > 47) return 255;
    uint8_t bits = resolution & 0xF;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if ((pinsStatus[ch].frequency != frequency) || (pinsStatus[ch].resolution != bits)) {
      configChannel(ch);
      ledcSetup(ch, frequency, bits);
      if (_sync) timerPause(ch);
      ledcWrite(ch, duty);
      pinsStatus[ch].frequency = frequency;
      pinsStatus[ch].resolution = bits;
      if (ch % 2 == 0) { // even ch
        pinsStatus[ch + 1].frequency = frequency;
        pinsStatus[ch + 1].resolution = bits;
      } else { // odd ch
        pinsStatus[ch - 1].frequency = frequency;
        pinsStatus[ch - 1].resolution = bits;
      }
    }
    if (pinsStatus[ch].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch].duty = duty;
    }
  }
  return pinsStatus[ch].frequency;
}

float Pwm::write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase) {
  uint8_t ch = getChannel(pin);
  if (ch < chMax) { // write PWM
    if ((pinsStatus[ch].pin) > 47) return 255;
    uint8_t bits = resolution & 0xF;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if ((pinsStatus[ch].frequency != frequency) || (pinsStatus[ch].resolution != bits)) {
      ledcSetup(ch, frequency, bits);
      if (_sync) timerPause(ch);
      ledcWrite(ch, duty);
      pinsStatus[ch].frequency = frequency;
      pinsStatus[ch].resolution = bits;
      if (ch % 2 == 0) { // even ch
        pinsStatus[ch + 1].frequency = frequency;
        pinsStatus[ch + 1].resolution = bits;
      } else { // odd ch
        pinsStatus[ch - 1].frequency = frequency;
        pinsStatus[ch - 1].resolution = bits;
      }
    }
    configChannel(ch);
    uint32_t ch_config = ch;
    if (ch > 7) ch_config = ch - 8;
    ledc_set_duty_with_hpoint((ledc_mode_t)pinsStatus[ch].mode, (ledc_channel_t)ch_config, duty, phase);
    pinsStatus[ch].phase = phase;
    if (pinsStatus[ch].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch].duty = duty;
    }
  }
  return pinsStatus[ch].frequency;
}
