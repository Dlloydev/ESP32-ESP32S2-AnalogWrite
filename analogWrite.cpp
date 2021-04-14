/**********************************************************************************
   AnalogWrite Library for ESP32-ESP32S2 Arduino core - Version 1.0.0
   by dlloydev https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
   This Library is licensed under the MIT License
 **********************************************************************************/

#include <Arduino.h>
#include "analogWrite.h"

pinStatus_t pinsStatus[16] = {
  { 1, -1, 5000, 13, 0 }, { 3, -1, 5000, 13, 0 },
  { 5, -1, 5000, 13, 0 }, { 7, -1, 5000, 13, 0 },
  { 9, -1, 5000, 13, 0 }, {11, -1, 5000, 13, 0 },
  {13, -1, 5000, 13, 0 }, {15, -1, 5000, 13, 0 }
};

void analogWrite(int8_t pin, int32_t value) {
  if (pin == DAC1 ||  pin == DAC2) { //dac
    if (value > 255) value = 255;
    dacWrite(pin, value);
  } else { //pwm
    int8_t ch = getChannel(pin);
    if (ch >= 0) {
      if (value == -1) { //detach pin
        pinsStatus[ch / 2].pin = -1;
        pinsStatus[ch / 2].frequency = 5000;
        pinsStatus[ch / 2].resolution = 13;
        ledcDetachPin(pinsStatus[ch / 2].pin);
        REG_SET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL, GPIO_MODE_DEF_DISABLE);
      } else {
        int32_t valueMax = (pow(2, pinsStatus[ch / 2].resolution)) - 1;
        if (value > valueMax) {
          value = valueMax + 1;
          ledcDetachPin(pin);
          pinMode(pin, OUTPUT);
          digitalWrite(pin, HIGH);
        } else {
          ledcSetup(ch, pinsStatus[ch / 2].frequency, pinsStatus[ch / 2].resolution);
          ledcWrite(ch, value);
        }
        pinsStatus[ch / 2].value = value;
      }
    }
  }
}

float analogWriteFrequency(int8_t pin, float frequency) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    pinsStatus[ch / 2].frequency = frequency;
    pinsStatus[ch / 2].pin = pin;
    ledcSetup(ch, frequency, pinsStatus[ch / 2].resolution);
    ledcWrite(ch, pinsStatus[ch / 2].value);
  }
  return ledcReadFreq(ch);
}

int32_t analogWriteResolution(int8_t pin, uint8_t resolution) {
  int8_t ch = getChannel(pin);
  if (ch >= 0) {
    pinsStatus[ch / 2].resolution = resolution;
    pinsStatus[ch / 2].pin = pin;
    ledcSetup(ch, pinsStatus[ch].frequency, resolution);
    ledcWrite(ch, pinsStatus[ch].value);
  }
  return pow(2, resolution);
}

int8_t getChannel(int8_t pin) {
  if ((pinMask >> pin) & 1) { //valid pin number?
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[pin], MCU_SEL)) { //gpio pin function?
      for (int8_t i = 0; i < 8; i++) { //search channels for the pin
        if (pinsStatus[i].pin == pin) { //pin found
          return pinsStatus[i].channel;
          break;
        }
      }
      return -99; //pin is being used externally
    } else { //pin is not used
      for (int8_t i = 0; i < 8; i++) { //search for free channel
        if (pinsStatus[i].pin == -1) { //channel is free
          pinsStatus[i].pin = pin;
          ledcAttachPin(pin, pinsStatus[i].channel);
          return pinsStatus[i].channel;
          break;
        }
      }
    }
  }
  return -88; //no available resources
}

void printPinsStatus() {
  Serial.print("PWM pins: ");
  for (int i = 0; i < muxSize; i++) {
    if ((pinMask >> i) & 1) {
      Serial.print(i); Serial.print(", ");
      if (i == 18) Serial.println();
      if (i == 18) Serial.print("          ");
    }
  }
  Serial.println();
  Serial.println();
  for (int i = 0; i < 8; i++) {
    Serial.print("ch"); Serial.print(pinsStatus[i].channel); Serial.print("  ");
    if (pinsStatus[i].channel < 10) Serial.print(" ");
    if (pinsStatus[i].pin == -1) {
      Serial.print(pinsStatus[i].pin); Serial.print("    ");
    } else {
      Serial.print("pin"); Serial.print(pinsStatus[i].pin); Serial.print("  ");
    }
    if (pinsStatus[i].pin < 10) Serial.print(" ");
    if (pinsStatus[i].frequency < 10000) Serial.print(" ");
    if (pinsStatus[i].frequency < 1000) Serial.print(" ");
    if (pinsStatus[i].frequency < 100) Serial.print(" ");
    if (pinsStatus[i].frequency < 10) Serial.print(" ");
    Serial.print(pinsStatus[i].frequency); Serial.print(" Hz  ");
    if (pinsStatus[i].resolution < 10) Serial.print(" ");
    Serial.print(pinsStatus[i].resolution); Serial.print("-bit  ");
    Serial.print("val ");
    if (pinsStatus[i].value < 10000) Serial.print(" ");
    if (pinsStatus[i].value < 1000) Serial.print(" ");
    if (pinsStatus[i].value < 100) Serial.print(" ");
    if (pinsStatus[i].value < 10) Serial.print(" ");
    Serial.print(pinsStatus[i].value);
    Serial.println();
  }
}
