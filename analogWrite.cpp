/**********************************************************************************
   AnalogWrite Library for ESP32-ESP32S2 Arduino core - Version 2.0.9
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 **********************************************************************************/
#include <Arduino.h>

#if (defined(ESP32) || defined(ARDUINO_ARCH_ESP32))
#include "analogWrite.h"

namespace aw {

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
pinStatus_t pinsStatus[8] = {
  {0, -1, 980, 8, 0, 0 }, {2, -1, 980, 8, 0, 0 },
  {4, -1, 980, 8, 0, 0 }, {6, -1, 980, 8, 0, 0 },
  {1, -1, 980, 8, 0, 0 }, {3, -1, 980, 8, 0, 0 },
  {5, -1, 980, 8, 0, 0 }, {7, -1, 980, 8, 0, 0 }
};
const uint8_t chd = 1;
#else //ESP32

pinStatus_t pinsStatus[8] = {
  { 0, -1, 980, 8, 0, 0 }, { 2, -1, 980, 8, 0, 0 },
  { 4, -1, 980, 8, 0, 0 }, { 6, -1, 980, 8, 0, 0 },
  { 8, -1, 980, 8, 0, 0 }, {10, -1, 980, 8, 0, 0 },
  {12, -1, 980, 8, 0, 0 }, {14, -1, 980, 8, 0, 0 }
};
const uint8_t chd = 2;
#endif

void awDetachPin(uint8_t pin, uint8_t ch) {
  pinsStatus[ch / chd].pin = -1;
  pinsStatus[ch / chd].value = 0;
  pinsStatus[ch / chd].frequency = 980;
  pinsStatus[ch / chd].resolution = 8;
  pinsStatus[ch / chd].phase = 0;
  ledcWrite(ch / chd, 0);
  ledcSetup(ch / chd, 0, 0);
  ledcDetachPin(pinsStatus[ch / chd].pin);
  REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
}

int8_t awGetChannel(int8_t pin) {
  if (!((pinMask >> pin) & 1)) return -1; //not pwm pin
  for (int8_t i = 0; i < 8; i++) {
    int8_t ch = pinsStatus[i].channel;
    if (pinsStatus[ch / chd].pin == pin) {
      return ch;
      break;
    }
  }
  for (int8_t i = 0; i < 8; i++) {
    int8_t ch = pinsStatus[i].channel;
    if ((REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) == 0) { //free pin
      if (pinsStatus[ch / chd].pin == -1) { //free channel
        if ((ledcRead(ch) < 1) && (ledcReadFreq(ch) < 1)) { //free timer
          pinsStatus[ch / chd].pin = pin;
          ledcSetup(ch, pinsStatus[ch / chd].frequency, pinsStatus[ch / chd].resolution);
          ledcAttachPin(pin, ch);
          return ch;
          break;
        } else {
          pinsStatus[ch / chd].pin = 88; //occupied timer
          return -1;
          break;
        }
      }
    } else {
      return -1; //occupied pin
      break;
    }
  }
  return -1;
}

} //namespace aw

float analogWrite(int8_t pin, int32_t value) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = aw::awGetChannel(pin);
    if (ch >= 0) {
      if (value == -1) aw::awDetachPin(pin, ch);
      else { // write PWM
        uint8_t bits = aw::pinsStatus[ch / aw::chd].resolution;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if (ledcRead(ch) != value) ledcWrite(ch, value);
        aw::pinsStatus[ch / aw::chd].value = value;
      }
    }
    return ledcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = aw::awGetChannel(pin);
    if (ch >= 0) {
      if ((aw::pinsStatus[ch / aw::chd].pin) > 47) return -1;
      if (value == -1) aw::awDetachPin(pin, ch);
      else { // write PWM
        uint8_t bits = aw::pinsStatus[ch / aw::chd].resolution;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if (aw::pinsStatus[ch / aw::chd].frequency != frequency) {
          ledcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].frequency = frequency;
        }
        if (aw::pinsStatus[ch / aw::chd].value != value) {
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].value = value;
        }
      }
    }
    return ledcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = aw::awGetChannel(pin);
    if (ch >= 0) {
      if ((aw::pinsStatus[ch / aw::chd].pin) > 47) return -1;
      if (value == -1) aw::awDetachPin(pin, ch);
      else { // write PWM
        uint8_t bits = resolution & 0xF;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if ((aw::pinsStatus[ch / aw::chd].frequency != frequency) || (aw::pinsStatus[ch / aw::chd].resolution != bits)) {
          ledcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].frequency = frequency;
          aw::pinsStatus[ch / aw::chd].resolution = bits;
        }
        if (aw::pinsStatus[ch / aw::chd].value != value) {
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].value = value;
        }
      }
    }
    return ledcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution, uint32_t phase) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = aw::awGetChannel(pin);
    if (ch >= 0) {
      if ((aw::pinsStatus[ch / aw::chd].pin) > 47) return -1;
      if (value == -1) aw::awDetachPin(pin, ch);
      else { // write PWM
        uint8_t bits = resolution & 0xF;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if ((aw::pinsStatus[ch / aw::chd].frequency != frequency) || (aw::pinsStatus[ch / aw::chd].resolution != bits)) {
          ledcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].frequency = frequency;
          aw::pinsStatus[ch / aw::chd].resolution = bits;
        }
        if (aw::pinsStatus[ch / aw::chd].phase != phase) {
          uint32_t group = (ch / 8), timer = ((ch / 2) % 4);
          aw::ledc_channel_config_t ledc_channel {
            (uint8_t)pin,
            (aw::ledc_mode_t)group,
            (aw::ledc_channel_t)ch,
            aw::LEDC_INTR_DISABLE,
            (aw::ledc_timer_t)timer,
            (uint32_t)value,
            (int)phase,
          };
          ledc_channel_config(&ledc_channel);
          ledc_set_duty_with_hpoint((aw::ledc_mode_t)group, (aw::ledc_channel_t)ch, value, phase);
          aw::pinsStatus[ch / aw::chd].phase = phase;
        }
        if (aw::pinsStatus[ch / aw::chd].value != value) {
          ledcWrite(ch, value);
          aw::pinsStatus[ch / aw::chd].value = value;
        }
      }
    }
    return ledcReadFreq(ch);
  }
  return 0;
}

float analogWriteFrequency(int8_t pin, float frequency) {
  int8_t ch = aw::awGetChannel(pin);
  if (ch >= 0) {
    if ((aw::pinsStatus[ch / aw::chd].pin) > 47) return -1;
    if (aw::pinsStatus[ch / aw::chd].frequency != frequency) {
      ledcSetup(ch, frequency, aw::pinsStatus[ch / aw::chd].resolution);
      ledcWrite(ch, aw::pinsStatus[ch / aw::chd].value);
      aw::pinsStatus[ch / aw::chd].frequency = frequency;
    }
  }
  return ledcReadFreq(ch);
}

int32_t analogWriteResolution(int8_t pin, uint8_t resolution) {
  int8_t ch = aw::awGetChannel(pin);
  if (ch >= 0) {
    if ((aw::pinsStatus[ch / aw::chd].pin) > 47) return -1;
    if (aw::pinsStatus[ch / aw::chd].resolution != resolution) {
      ledcDetachPin(pin);
      ledcSetup(ch, aw::pinsStatus[ch / aw::chd].frequency, resolution & 0xF);
      ledcAttachPin(pin, ch);
      ledcWrite(ch, aw::pinsStatus[ch / aw::chd].value);
      aw::pinsStatus[ch / aw::chd].resolution = resolution & 0xF;
    }
  }
  return 1 << (resolution & 0xF);
}

void setPinsStatusDefaults(int32_t value, float frequency, uint8_t resolution, uint32_t phase) {
  for (int8_t i = 0; i < 8; i++) {
    aw::pinsStatus[i].value = value;
    aw::pinsStatus[i].frequency = frequency;
    aw::pinsStatus[i].resolution = resolution;
    aw::pinsStatus[i].phase = phase;
  }
}

void printPinsStatus() {
  Serial.print(F("PWM pins: "));
  for (int i = 0; i < aw::muxSize; i++) {
    if ((aw::pinMask >> i) & 1) {
      Serial.print(i); Serial.print(F(", "));
    }
  }
  Serial.println();

  Serial.println();
  for (int i = 0; i < 8; i++) {
    int ch = aw::pinsStatus[i].channel;
    Serial.print(F("ch: "));
    if (ch < 10) Serial.print(F(" ")); Serial.print(ch); Serial.print(F("  "));
    Serial.print(F("Pin: "));
    if ((aw::pinsStatus[ch / aw::chd].pin >= 0) && (aw::pinsStatus[ch / aw::chd].pin < 10)) Serial.print(F(" "));
    Serial.print(aw::pinsStatus[ch / aw::chd].pin); Serial.print(F("  "));
    Serial.print(F("Hz: "));
    if (ledcReadFreq(ch) < 10000) Serial.print(F(" "));
    if (ledcReadFreq(ch) < 1000) Serial.print(F(" "));
    if (ledcReadFreq(ch) < 100) Serial.print(F(" "));
    if (ledcReadFreq(ch) < 10) Serial.print(F(" "));
    Serial.print(ledcReadFreq(ch)); Serial.print(F("  "));
    Serial.print(F("Bits: "));
    if (aw::pinsStatus[ch / aw::chd].resolution < 10) Serial.print(F(" "));
    Serial.print(aw::pinsStatus[ch / aw::chd].resolution); Serial.print(F("  "));
    Serial.print(F("Duty: "));
    if (aw::pinsStatus[ch / aw::chd].value < 10000) Serial.print(F(" "));
    if (aw::pinsStatus[ch / aw::chd].value < 1000) Serial.print(F(" "));
    if (aw::pinsStatus[ch / aw::chd].value < 100) Serial.print(F(" "));
    if (aw::pinsStatus[ch / aw::chd].value < 10) Serial.print(F(" "));
    Serial.print(aw::pinsStatus[ch / aw::chd].value); Serial.print(F("  "));
    Serial.print(F("Ø: "));
    if (aw::pinsStatus[ch / aw::chd].phase < 1000) Serial.print(F(" "));
    if (aw::pinsStatus[ch / aw::chd].phase < 100) Serial.print(F(" "));
    if (aw::pinsStatus[ch / aw::chd].phase < 10) Serial.print(F(" "));
    Serial.print(aw::pinsStatus[ch / aw::chd].phase);
    Serial.println();
  }
}
#endif //ESP32
