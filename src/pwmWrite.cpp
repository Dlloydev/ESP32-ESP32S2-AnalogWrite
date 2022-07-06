/*******************************************************************
   pwmWrite Library for ESP32 Arduino core, Version 3.0.3
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 *******************************************************************/

#include <Arduino.h>
#include "pwmWrite.h"
#include <stdlib.h>

Pwm::Pwm() {}

void Pwm::detachPin(uint8_t pin, uint8_t ch) {
  pinsStatus[ch / chd].pin = 255;
  pinsStatus[ch / chd].duty = 0;
  pinsStatus[ch / chd].frequency = 1000;
  pinsStatus[ch / chd].resolution = 8;
  pinsStatus[ch / chd].phase = 0;
  ledcWrite(ch / chd, 0);
  ledcSetup(ch / chd, 0, 0);
  ledcDetachPin(pinsStatus[ch / chd].pin);
  REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
}

uint8_t Pwm::getChannel(uint8_t pin) {
  if (!((pinMask >> pin) & 1)) return 254; //not pwm pin
  for (int8_t i = 0; i < 8; i++) {
    uint8_t ch = pinsStatus[i].channel;
    if (pinsStatus[ch / chd].pin == pin) {
      return ch;
      break;
    }
  }
  for (int8_t i = 0; i < 8; i++) {
    uint8_t ch = pinsStatus[i].channel;
    if ((REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) == 0) { //free pin
      if (pinsStatus[ch / chd].pin == 255) { //free channel
        pinsStatus[ch / chd].pin = pin;
        ledcSetup(ch, pinsStatus[ch / chd].frequency, pinsStatus[ch / chd].resolution);
        ledcAttachPin(pin, ch);
        return ch;
        break;
      }
    } else {
      return 253; //occupied pin
      break;
    }
  }
  return 252;
}

float Pwm::writeFrequency(uint8_t pin, float frequency) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    if (pinsStatus[ch / chd].frequency != frequency) {
      ledcSetup(ch, frequency, pinsStatus[ch / chd].resolution);
      ledcWrite(ch, pinsStatus[ch / chd].duty);
      pinsStatus[ch / chd].frequency = frequency;
    }
  }
  return ledcReadFreq(ch);
}

int32_t Pwm::writeResolution(uint8_t pin, uint8_t resolution) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    if (pinsStatus[ch / chd].resolution != resolution) {
      ledcDetachPin(pin);
      ledcSetup(ch, pinsStatus[ch / chd].frequency, resolution & 0xF);
      ledcAttachPin(pin, ch);
      ledcWrite(ch, pinsStatus[ch / chd].duty);
      pinsStatus[ch / chd].resolution = resolution & 0xF;
    }
  }
  return 1 << (resolution & 0xF);
}

void Pwm::setPinsStatusDefaults(int32_t duty, float frequency, uint8_t resolution, uint32_t phase) {
  for (int8_t i = 0; i < 8; i++) {
    pinsStatus[i].duty = duty;
    pinsStatus[i].frequency = frequency;
    pinsStatus[i].resolution = resolution;
    pinsStatus[i].phase = phase;
  }
}

void Pwm::printPinsStatus() {
  Serial.print(F("PWM pins: "));
  for (int i = 0; i < muxSize; i++) {
    if ((pinMask >> i) & 1) {
      Serial.print(i); Serial.print(F(", "));
    }
  }
  Serial.println(); Serial.println();
  for (int i = 0; i < 8; i++) {
    uint8_t ch = pinsStatus[i].channel;
    Serial.print(F("ch: "));
    if (ch < 10) Serial.print(F(" ")); Serial.print(ch); Serial.print(F("  "));
    Serial.print(F("Pin: "));
    if (pinsStatus[ch / chd].pin < 100) Serial.print(F(" "));
    if (pinsStatus[ch / chd].pin < 10) Serial.print(F(" "));
    Serial.print(pinsStatus[ch / chd].pin); Serial.print(F("  "));
    Serial.print(F("Hz: "));
    if (pinsStatus[ch / chd].frequency < 10000) Serial.print(F(" "));
    if (pinsStatus[ch / chd].frequency < 1000) Serial.print(F(" "));
    if (pinsStatus[ch / chd].frequency < 100) Serial.print(F(" "));
    if (pinsStatus[ch / chd].frequency < 10) Serial.print(F(" "));
    Serial.print(pinsStatus[ch / chd].frequency); Serial.print(F("  "));
    Serial.print(F("Bits: "));
    if (pinsStatus[ch / chd].resolution < 10) Serial.print(F(" "));
    Serial.print(pinsStatus[ch / chd].resolution); Serial.print(F("  "));
    Serial.print(F("Duty: "));
    if (pinsStatus[ch / chd].duty < 10000) Serial.print(F(" "));
    if (pinsStatus[ch / chd].duty < 1000) Serial.print(F(" "));
    if (pinsStatus[ch / chd].duty < 100) Serial.print(F(" "));
    if (pinsStatus[ch / chd].duty < 10) Serial.print(F(" "));
    Serial.print(pinsStatus[ch / chd].duty); Serial.print(F("  "));
    Serial.print(F("Ø: "));
    if (pinsStatus[ch / chd].phase < 1000) Serial.print(F(" "));
    if (pinsStatus[ch / chd].phase < 100) Serial.print(F(" "));
    if (pinsStatus[ch / chd].phase < 10) Serial.print(F(" "));
    Serial.print(pinsStatus[ch / chd].phase);
    Serial.println();
  }
}

float Pwm::write(uint8_t pin, int32_t duty) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) { // write PWM
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    uint8_t bits = pinsStatus[ch / chd].resolution;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if (ledcRead(ch) != duty) ledcWrite(ch, duty);
    pinsStatus[ch / chd].duty = duty;
  }
  return pinsStatus[ch / chd].frequency;
}

float Pwm::write(uint8_t pin, int32_t duty, float frequency) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) { // write PWM
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    uint8_t bits = pinsStatus[ch / chd].resolution;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if (pinsStatus[ch / chd].frequency != frequency) {
      ledcSetup(ch, frequency, bits);
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].frequency = frequency;
    }
    if (pinsStatus[ch / chd].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].duty = duty;
    }
  }
  return pinsStatus[ch / chd].frequency;
}

float Pwm::write(uint8_t pin, int32_t duty, float frequency, uint8_t resolution) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) { // write PWM
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    uint8_t bits = resolution & 0xF;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if ((pinsStatus[ch / chd].frequency != frequency) || (pinsStatus[ch / chd].resolution != bits)) {
      ledcSetup(ch, frequency, bits);
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].frequency = frequency;
      pinsStatus[ch / chd].resolution = bits;
    }
    if (pinsStatus[ch / chd].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].duty = duty;
    }
  }
  return pinsStatus[ch / chd].frequency;
}

float Pwm::write(uint8_t pin, int32_t duty, float frequency, uint8_t resolution, uint32_t phase) {
  uint8_t ch = getChannel(pin);
  if (ch >= 0) { // write PWM
    if ((pinsStatus[ch / chd].pin) > 47) return 255;
    uint8_t bits = resolution & 0xF;
    if (duty > ((1 << bits) - 1)) duty = (1 << bits); //constrain
    if ((bits > 7) && (duty == ((1 << bits) - 1))) duty = (1 << bits); //keep PWM high
    if ((pinsStatus[ch / chd].frequency != frequency) || (pinsStatus[ch / chd].resolution != bits)) {
      ledcSetup(ch, frequency, bits);
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].frequency = frequency;
      pinsStatus[ch / chd].resolution = bits;
    }
    if (pinsStatus[ch / chd].phase != phase) {
      uint32_t group = (pinsStatus[ch / chd].group);
      uint32_t timer = (pinsStatus[ch / chd].timer);

      ledc_channel_config_t ledc_channel = {
        .gpio_num       = (pinsStatus[ch / chd].pin),
        .speed_mode     = (ledc_mode_t) group,
        .channel        = (ledc_channel_t) ch,
        .intr_type      = (ledc_intr_type_t) LEDC_INTR_DISABLE,
        .timer_sel      = (ledc_timer_t) timer,
        .duty           = 0,
        .hpoint         = 0,
        .flags = {
          .output_invert = 0
        }
      };
      ledc_channel_config(&ledc_channel);
      ledc_set_duty_with_hpoint((ledc_mode_t)group, (ledc_channel_t)ch, duty, phase);

      pinsStatus[ch / chd].phase = phase;
    }
    if (pinsStatus[ch / chd].duty != duty) {
      ledcWrite(ch, duty);
      pinsStatus[ch / chd].duty = duty;
    }
  }
  return pinsStatus[ch / chd].frequency;
}
