/**********************************************************************************
   AnalogWrite Library for ESP32-ESP32S2 Arduino core - Version 1.1.0
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 **********************************************************************************/

#include <Arduino.h>
#include "analogWrite.h"

#if (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
pinStatus_t pinsStatus[8] = {
  {0, -1, 5000, 13, 0 }, {2, -1, 5000, 13, 0 },
  {4, -1, 5000, 13, 0 }, {6, -1, 5000, 13, 0 },
  {1, -1, 5000, 13, 0 }, {3, -1, 5000, 13, 0 },
  {5, -1, 5000, 13, 0 }, {7, -1, 5000, 13, 0 }
};
const uint8_t chd = 1;
#else //ESP32
pinStatus_t pinsStatus[8] = {
  { 0, -1, 5000, 13, 0 }, { 2, -1, 5000, 13, 0 },
  { 4, -1, 5000, 13, 0 }, { 6, -1, 5000, 13, 0 },
  { 8, -1, 5000, 13, 0 }, {10, -1, 5000, 13, 0 },
  {12, -1, 5000, 13, 0 }, {14, -1, 5000, 13, 0 }
};
const uint8_t chd = 2;
#endif

void analogWrite(int8_t pin, int32_t value) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else { //pwm
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if (value == -1) { //detach pin
        pinsStatus[ch / chd].pin = -1;
        pinsStatus[ch / chd].frequency = 5000;
        pinsStatus[ch / chd].resolution = 13;
        ledcDetachPin(pinsStatus[ch / chd].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else { // attached
        int32_t valueMax = (pow(2, pinsStatus[ch / chd].resolution)) - 1;
        if (value > valueMax) { // full ON
          value = valueMax + 1;
          ledcDetachPin(pin);
          pinMode(pin, OUTPUT);
          digitalWrite(pin, HIGH);
        } else { // write PWM
          ledcSetup(ch, pinsStatus[ch / chd].frequency, pinsStatus[ch / chd].resolution);
          ledcWrite(ch, value);
        }
        pinsStatus[ch / chd].value = value;
      }
    }
  }
}

float analogWriteFrequency(int8_t pin, float frequency) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return -1;
    pinsStatus[ch / chd].pin = pin;
    pinsStatus[ch / chd].frequency = frequency;
    //ledcChangeFrequency(ch, frequency, pinsStatus[ch / chd].resolution);
    ledcSetup(ch, frequency, pinsStatus[ch / chd].resolution);
    ledcWrite(ch, pinsStatus[ch / chd].value);
  }
  return ledcReadFreq(ch);
}

int32_t analogWriteResolution(int8_t pin, uint8_t resolution) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    if ((pinsStatus[ch / chd].pin) > 47) return -1;
    pinsStatus[ch / chd].pin = pin;
    pinsStatus[ch / chd].resolution = resolution;
    ledcSetup(ch, pinsStatus[ch / chd].frequency, resolution);
    ledcWrite(ch, pinsStatus[ch / chd].value);
  }
  return pow(2, resolution);
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
    if (ledcReadFreq(ch) < 10000) Serial.print(" ");
    if (ledcReadFreq(ch) < 1000) Serial.print(" ");
    if (ledcReadFreq(ch) < 100) Serial.print(" ");
    if (ledcReadFreq(ch) < 10) Serial.print(" ");
    Serial.print(ledcReadFreq(ch)); Serial.print("  ");
    Serial.print("Bits: ");
    if (pinsStatus[ch / chd].resolution < 10) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].resolution); Serial.print("  ");
    Serial.print("Duty: ");
    if (pinsStatus[ch / chd].value < 10000) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 1000) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 100) Serial.print(" ");
    if (pinsStatus[ch / chd].value < 10) Serial.print(" ");
    Serial.print(pinsStatus[ch / chd].value);
    Serial.println();
  }
}
