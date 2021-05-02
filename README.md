

## analogWrite()

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)

![image](https://user-images.githubusercontent.com/63488701/116161369-a4e06380-a6c1-11eb-995a-55f2ff7393c0.png)

### ESP32 Installation Instructions

This library was tested using using the [ESP32 Arduino IDE Boards Manager](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md) installation method.

- Stable release link:  tested with ESP32 Dev Module
- Development release link: Tested with ESP32 Dev Module and ESP32-S2 Dev Module

### Description

The `analogWrite()` function writes a duty cycle value to a ([PWM wave](http://arduino.cc/en/Tutorial/PWM)) on a digital pin or an analog value to a DAC pin.  It wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 8 PWM channels and works with the 2 DAC pins.  A unique feature is its friendly and co-operative pin management where it will not attach to and use a pin that has been previously accessed by other code including any of the Arduino pin I/O functions. The use of this function is similar to the Arduino method as its resource management is handled transparently (you do not need to call `pinMode()`prior to use).

Now analogWrite can assign a pin and contol PWM duty value, frequency, resolution and **phase** all from one function. This function now returns the PWM frequency reported from the ledc framework.

### [Using AnalogWrite](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/Using%20AnalogWrite.md)

### 3-phase PWM Example

Details: 3-pins (4, 5 and 12), 10-bit PWM split into 3 equal ON-periods  of 341. Frequency is 100Hz. Signal on pin 5 is phase shifted by 341  steps, signal on pin 12 is shifted by 682 steps.

```apl
  analogWrite(4, 341, 100, 10, 0);
  analogWrite(5, 341, 100, 10, 341);
  analogWrite(12, 341, 100, 10, 682);
```

![image](https://user-images.githubusercontent.com/63488701/115972875-1207c380-a51f-11eb-8051-c7d39b9144ab.png)

### PWM Wave

Arduino's reference for [`analogWrite()`](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/) describes the PWM wave characteristics for various hardware architecture.  The general operational characteristic is  8-bit duty cycle control where the output will be **always off** for value 0 and **always on** for value 255.  The ESP32's LEDc PWM does not turn fully on at the max duty value, i.e. for 8-bit resolution, the maximim duty is 255/256, but now this is corrected in software for 8-bits and higher resolution. For 1-7 bit resolution settings, the correction is not made and AnalogWrite works the same as the LEDc PWM control functions.

When using ESP32S2 devices, the core is in early development and the max frequency and bit resolutions are somewhat limited. On these devices, tested range is 8-bit: 4Hz to 2.5kHz, 13-bit 0.2Hz to 120Hz.

| Board   | PWM Pins                          | DAC Pins   | PWM Frequency  | Resolution                  |
| ------- | --------------------------------- | ---------- | -------------- | --------------------------- |
| ESP32   | 2, 4, 5, 12-19, 21-23, 27, 32, 33 | DAC1, DAC2 | 980 Hz default | 1-16 bit PWM, 8-bit default |
| ESP32S2 | 1- 14, 21, 33-42, 45              | DAC1, DAC2 | 980 Hz default | 1-16 bit PWM, 8-bit default |

### AnalogWrite Options

```c++
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution, uint32_t phase);
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution);
float analogWrite(int8_t pin, int32_t value, float frequency);
float analogWrite(int8_t pin, int32_t value);
```

### Basic Syntax

```c++
float analogWrite(pin, value);
```

### Parameters

`pin`: The GPIO pin to write to.  Allowed data types: `int`.
`value`: The duty cycle between 0 (always off) and `pow(2, resolution)` (always on). This function automatically attaches the pin to the first available channel. To avoid conflicts with other code, the chosen pin will not be available if it was previously accessed by other code. If you need to release a pin that analogWrite has previously used, just use the command `analogWrite(pin, -1);` The return value is the PWM frequency reported by the LEDc methods. Various overload functions are provided (shown above) allowing the user to use only the parameters needed at any time.

### analogWriteFrequency()

```c++
float analogWriteFrequency(int8_t pin, float frequency);
```

Sets the PWM frequency (default 980Hz) on any PWM pin. Returns the exact hardware frequency used by the timer.

### analogWriteResolution()

```c++
int32_t analogWriteResolution(int8_t pin, uint8_t resolution);
```

The PWM resolution for any PWM pin can be set from 1-bit to 16-bit (default 8-bit). This has no effect on the DAC pins which are 8-bit only. Returns the maximum value that will be always on for the selected resolution: `pow(2, resolution)`

### setPinsStatusDefaults()

```c++
void setPinsStatusDefaults(int32_t value = 0, float frequency = 980, uint8_t resolution = 8, uint32_t phase = 0);
```

This function sets the default PWM value, frequency, resolution and phase for all 8 channels. If called with no arguments, the defaults are as shown above, which matches the Arduino UNO PWM defaults.  This is also the same as the startup defaults, so it's not a requirement to call this function unless new defaults are required.

### printPinsStatus()

```c++
void printPinsStatus();
```

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are available (-1). Resource management is handled automatically and transparently. There is no requirement to have any function in the setup loop. PWM resolution and frequency can be changed at any time in the main loop. If you enter a higher duty value than the limit, the status output will show the max value that has effect.

| ESP32 Dev Board                                              | ESP32S2-Saola-1M                                             |
| :----------------------------------------------------------- | ------------------------------------------------------------ |
| ![image](https://user-images.githubusercontent.com/63488701/116158380-92176000-a6bc-11eb-901d-87a7cfb8ba93.png) | ![image](https://user-images.githubusercontent.com/63488701/116162591-1e795100-a6c4-11eb-9717-6f63a3abca06.png) |
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

