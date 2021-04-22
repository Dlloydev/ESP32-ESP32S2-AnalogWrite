

## analogWrite()

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)

![image](https://user-images.githubusercontent.com/63488701/114867299-9db47e00-9dc2-11eb-953c-7004a92e8a18.png)

### Description

The `analogWrite()` function writes a duty cycle value to a ([PWM wave](http://arduino.cc/en/Tutorial/PWM)) on a digital pin or an analog value to a DAC pin.  It wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 8 PWM channels plus it works with the 2 DAC pins.  A unique feature is its friendly and co-operative pin management where it will not attach to and use a pin that has been previously accessed by other code including any of the Arduino pin I/O functions. 

The use of this function is similar to the Arduino method as its resource management is handled transparently (you do not need to call `pinMode()`prior to use). The `analogWriteResolution()` and `analogWriteFrequency()` functions are provided to give independent control over bit width configuration and PWM frequency.

### PWM Wave

Arduino's reference for [`analogWrite()`](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/) describes the PWM wave characteristics for various hardware architecture.  The general operational characteristic is  8-bit duty cycle control where the output will be **always off** for value 0 and **always on** for value 255. With the various devices and timer modes, sometimes a bit correction is required to achieve full off or on. The ESP8266 follows this mode of operation, but with the different timer architecture on the ESP32 devices, the **LEDc** PWM operates in a different manner, where duty value 0 is always off, but duty value 255 will give an output that's 255/256 duty cycle (not fully on).

As of version 1.2.1, this condition is detected and corrected, where the hardware is programmed with `2^resolution (bits)^`, which drives the output signal fully on for 8-bit and higher resolution settings. This full 0-100% control will now completely turn on/off both common anode and common cathode LED devices. For 1-7 bit resolution settings, the max value correction is not made and AnalogWrite works the same as the LEDc PWM control functions.

When using ESP32S2 devices, this version offers a temporary fix for ***ESP32-S2 PWM for a Servo pulse issue #5050*** but with limited frequency range. Tested range is 8-bit: 4Hz to 2.5kHz, 13-bit 0.2Hz to 120Hz.

| Board   | PWM Pins                          | DAC Pins   | PWM Frequency   | Resolution              |
| ------- | --------------------------------- | ---------- | --------------- | ----------------------- |
| ESP32   | 2, 4, 5, 12-19, 21-23, 27, 32, 33 | DAC1, DAC2 | 5000 Hz default | 1-16 bit PWM, 8-bit DAC |
| ESP32S2 | 1- 14, 21, 33-42, 45              | DAC1, DAC2 | 5000 Hz default | 1-16 bit PWM, 8-bit DAC |

### Syntax

```c++
void analogWrite(pin, value);
```

### Parameters

`pin`: The GPIO pin to write to.  Allowed data types: `int`.
`value`: The duty cycle between 0 (always off) and `pow(2, resolution)` (always on). With default 13-bit resolution, 8192 is always on.  This function automatically attaches the pin to the next available channel. To avoid conflicts with other code, the chosen pin will not be available if it was previously accessed by other code. If you need to release a pin that analogWrite has previously used, just use the command `analogWrite(pin, -1);` There is no return value.

### analogWriteFrequency()

```c++
float analogWriteFrequency(int8_t pin, float frequency);
```

Sets the PWM frequency (default 5000Hz) on any PWM pin. Returns the exact hardware frequency used by the timer.

### analogWriteResolution()

```c++
int32_t analogWriteResolution(int8_t pin, uint8_t resolution);
```

The PWM resolution for any PWM pin can be set from 1-bit to 16-bit. This has no effect on the DAC pins which are 8-bit only. Returns the maximum value that will be always on for the selected resolution: `pow(2, resolution)`

### printPinsStatus()

```c++
void printPinsStatus();
```

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are available (-1). Resource management is handled automatically and transparently. There is no requirement to have any function in the setup loop. PWM resolution and frequency can be changed at any time in the main loop. If you enter a higher duty value than the limit, the status output will show the max value that has effect.

| ESP32 Dev Board                                              | ESP32S2-Saola-1M                                             |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| ![esp32-pinsStatus-large](https://user-images.githubusercontent.com/63488701/115168460-f9b02880-a088-11eb-8c65-debeee7a7858.png) | ![esp32-s2 pinsStatus-large](https://user-images.githubusercontent.com/63488701/115168504-26644000-a089-11eb-93f1-476fdf1b4418.png) |
| All 8 channels offer independent resolution bits, duty cycle value and frequency . | All 8 channels offer independent resolution bits, duty cycle value. Four independant frequencies on channels (0,1), (2,3), (4,5) and (6,7) |
| `const uint64_t pinMask = 0x27FE00207FFE;`                   | `const uint64_t pinMask = 0x308EFF034;`                      |

The  the available PWM pins are determined by a pinMask constant. It might be necessary to alter the pimMask to match your board  or to customize for your design.

### Example Code

- [AnalogWriteTest](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/AnalogWriteTest/AnalogWriteTest.ino)
- [Fade](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/tree/main/examples/Fade)

### Notes

Both timer resolution and PWM frequency should be calculated to get expected results. Refer to [Supported Range of Frequency and Duty Resolution](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution) as a reference. 

- Up to 8 PWM channels and 2 DAC pins are managed transparently
- If required, a channel can be freed  up by using `analogWrite(pin, -1)`
- To co-operate with other code, `analogWrite()` will not work with any pin that has been previously accessed 

```
This Library is licensed under the MIT License
```

