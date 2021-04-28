/**********************************************************************************
   AnalogWrite Library for ESP32-ESP32S2 Arduino core - Version 2.0.3
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 **********************************************************************************/

#include <Arduino.h>
#include "analogWrite.h"

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
pinStatus_t pinsStatus[8] = {
  {0, -1, 5000, 13, 0, 0 }, {2, -1, 5000, 13, 0, 0 },
  {4, -1, 5000, 13, 0, 0 }, {6, -1, 5000, 13, 0, 0 },
  {1, -1, 5000, 13, 0, 0 }, {3, -1, 5000, 13, 0, 0 },
  {5, -1, 5000, 13, 0, 0 }, {7, -1, 5000, 13, 0, 0 }
};
const uint8_t chd = 1;
#else //ESP32
pinStatus_t pinsStatus[8] = {
  { 0, -1, 5000, 13, 0, 0 }, { 2, -1, 5000, 13, 0, 0 },
  { 4, -1, 5000, 13, 0, 0 }, { 6, -1, 5000, 13, 0, 0 },
  { 8, -1, 5000, 13, 0, 0 }, {10, -1, 5000, 13, 0, 0 },
  {12, -1, 5000, 13, 0, 0 }, {14, -1, 5000, 13, 0, 0 }
};
const uint8_t chd = 2;
#endif

float analogWrite(int8_t pin, int32_t value) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if (value == -1) { //detach pin
        pinsStatus[ch / chd].pin = -1;
        pinsStatus[ch / chd].frequency = 5000;
        pinsStatus[ch / chd].resolution = 13;
        ledcDetachPin(pinsStatus[ch / chd].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else { // write PWM
        uint8_t bits = pinsStatus[ch / chd].resolution;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if (ledcRead(ch) != value) ledcWrite(ch, value);
        pinsStatus[ch / chd].value = value;
      }
    }
    return awLedcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if ((pinsStatus[ch / chd].pin) > 47) return -1;
      if (value == -1) { //detach pin
        pinsStatus[ch / chd].pin = -1;
        pinsStatus[ch / chd].frequency = 5000;
        pinsStatus[ch / chd].resolution = 13;
        ledcDetachPin(pinsStatus[ch / chd].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else { // write PWM
        uint8_t bits = pinsStatus[ch / chd].resolution;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if (pinsStatus[ch / chd].frequency != frequency) {
          awLedcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          pinsStatus[ch / chd].frequency = frequency;
        }
        if (ledcRead(ch) != value) {
          ledcWrite(ch, value);
          pinsStatus[ch / chd].value = value;
        }
      }
    }
    return awLedcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if ((pinsStatus[ch / chd].pin) > 47) return -1;
      if (value == -1) { //detach pin
        pinsStatus[ch / chd].pin = -1;
        pinsStatus[ch / chd].frequency = 5000;
        pinsStatus[ch / chd].resolution = 13;
        ledcDetachPin(pinsStatus[ch / chd].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else { // write PWM
        uint8_t bits = resolution & 0xF;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if ((pinsStatus[ch / chd].frequency != frequency) || (pinsStatus[ch / chd].resolution != bits)) {
          awLedcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          pinsStatus[ch / chd].frequency = frequency;
          pinsStatus[ch / chd].resolution = bits;
        }
        if (ledcRead(ch) != value) {
          ledcWrite(ch, value);
          pinsStatus[ch / chd].value = value;
        }
      }
    }
    return awLedcReadFreq(ch);
  }
  return 0;
}

float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution, uint32_t phase) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else {
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if ((pinsStatus[ch / chd].pin) > 47) return -1;
      if (value == -1) { //detach pin
        pinsStatus[ch / chd].pin = -1;
        pinsStatus[ch / chd].frequency = 5000;
        pinsStatus[ch / chd].resolution = 13;
        ledcDetachPin(pinsStatus[ch / chd].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else { // write PWM
        uint8_t bits = resolution & 0xF;
        if (value > ((1 << bits) - 1)) value = (1 << bits); //constrain
        if ((bits > 7) && (value == ((1 << bits) - 1))) value = (1 << bits); //keep PWM high
        if ((pinsStatus[ch / chd].frequency != frequency) || (pinsStatus[ch / chd].resolution != bits)) {
          awLedcSetup(ch, frequency, bits);
          ledcWrite(ch, value);
          pinsStatus[ch / chd].frequency = frequency;
          pinsStatus[ch / chd].resolution = bits;
        }
        if (pinsStatus[ch / chd].phase != phase) {
          uint32_t group = (ch / 8), timer = ((ch / 2) % 4);
          ledc_channel_config_t ledc_channel {
            (uint8_t)pin,
            (ledc_mode_t)group,
            (ledc_channel_t)ch,
            LEDC_INTR_DISABLE,
            (ledc_timer_t)timer,
            (uint32_t)value,
            (int)phase,
          };
          ledc_channel_config(&ledc_channel);
          ledc_set_duty_with_hpoint((ledc_mode_t)group, (ledc_channel_t)ch, value, phase);
          pinsStatus[ch / chd].phase = phase;
        }
        if (ledcRead(ch) != value) {
          ledcWrite(ch, value);
          pinsStatus[ch / chd].value = value;
        }
      }
    }
    return awLedcReadFreq(ch);
  }
  return 0;
}

float analogWriteFrequency(int8_t pin, float frequency) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return -1;
    pinsStatus[ch / chd].pin = pin;
    if (pinsStatus[ch / chd].frequency != frequency) {
      awLedcSetup(ch, frequency, pinsStatus[ch / chd].resolution);
      ledcWrite(ch, pinsStatus[ch / chd].value);
      pinsStatus[ch / chd].frequency = frequency;
    }
  }
  return awLedcReadFreq(ch);
}

int32_t analogWriteResolution(int8_t pin, uint8_t resolution) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return -1;
    pinsStatus[ch / chd].pin = pin;
    if (pinsStatus[ch / chd].resolution != resolution) {
      awLedcSetup(ch, pinsStatus[ch / chd].frequency, resolution & 0xF);
      ledcWrite(ch, pinsStatus[ch / chd].value);
      pinsStatus[ch / chd].resolution = resolution & 0xF;
    }
  }
  return 1 << resolution & 0xF;
}

float awLedcSetup(uint8_t ch, double frequency, uint8_t bits) {
#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
  frequency *= 80; //workaround for issue 5050
  return ledcSetup(ch, frequency, bits) / 80;
#else //ESP32
  return ledcSetup(ch, frequency, bits);
#endif
}

float awLedcReadFreq(uint8_t ch) {
#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
  return ledcReadFreq(ch) / 80; //workaround for issue 5050
#else //ESP32
  return ledcReadFreq(ch);
#endif
}

int8_t getChannel(int8_t pin) {
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
          awLedcSetup(ch, pinsStatus[ch / chd].frequency, pinsStatus[ch / chd].resolution);
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

void printPinsStatus() {
  Serial.print("PWM pins: ");
  for (int i = 0; i < muxSize; i++) {
    if ((pinMask >> i) & 1) {
      Serial.print(i); Serial.print(", ");
    }
  }
  Serial.println();

  Serial.println();
  for (int i = 0; i < 8; i++) {
    int ch = pinsStatus[i].channel;
    Serial.print("ch: ");
    if (ch < 10) Serial.print(" "); Serial.print(ch); Serial.print("  ");
    Serial.print("Pin: ");
    if ((pinsStatus[ch / chd].pin >= 0) && (pinsStatus[ch / chd].pin < 10)) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].pin); Serial.print("  ");
    Serial.print("Hz: ");
    if (awLedcReadFreq(ch) < 10000) Serial.print(" ");
    if (awLedcReadFreq(ch) < 1000) Serial.print(" ");
    if (awLedcReadFreq(ch) < 100) Serial.print(" ");
    if (awLedcReadFreq(ch) < 10) Serial.print(" ");
    Serial.print(awLedcReadFreq(ch)); Serial.print("  ");
    Serial.print("Bits: ");
    if (pinsStatus[ch / chd].resolution < 10) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].resolution); Serial.print("  ");
    Serial.print("Duty: ");
    if (pinsStatus[ch / chd].value < 10000) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 1000) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 100) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 10) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].value); Serial.print("  ");
    Serial.print("Ø: ");
    if (pinsStatus[ch / chd].phase < 1000) Serial.print(" ");
    if (pinsStatus[ch / chd].phase < 100) Serial.print(" ");
    if (pinsStatus[ch / chd].phase < 10) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].phase);
    Serial.println();
  }
}
