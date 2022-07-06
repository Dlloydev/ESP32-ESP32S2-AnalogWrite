



# pwm.write()

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)  <a href="https://registry.platformio.org/libraries/dlloydev/ESP32 ESP32S2 AnalogWrite"><img src="https://badges.registry.platformio.org/packages/dlloydev/library/ESP32 ESP32S2 AnalogWrite.svg" alt="PlatformIO Registry" /></a>

![image](https://user-images.githubusercontent.com/63488701/174445314-c7945015-f295-4cba-917c-cc4ead8d534a.png)

#### *Wokwi phase shift examples ... [ESP32](https://wokwi.com/projects/334722465700774482) ... [ESP32-S2](https://wokwi.com/projects/334765722024542804)*

### Description

`pwm.write()` wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 8 PWM channels.  A unique feature is its friendly and co-operative pin management where it will not attach to and use a pin that has been previously accessed by other code including any of the Arduino pin I/O functions. The resource management is handled transparently. You do not need to call `pinMode()`prior to use. This function can assign a pin and contol PWM duty value, frequency, resolution and **phase** all from one function.

| Board    | PWM Pins                          | PWM Frequency   | Resolution                  |
| -------- | --------------------------------- | --------------- | --------------------------- |
| ESP32    | 2, 4, 5, 12-19, 21-23, 27, 32, 33 | 1000 Hz default | 1-16 bit PWM, 8-bit default |
| ESP32-S2 | 1- 14, 21, 33-42, 45              | 1000 Hz default | 1-16 bit PWM, 8-bit default |
| ESP32-C3 | 0- 9, 18, 19                      | 1000 Hz default | 1-16 bit PWM, 8-bit default |

### pwm.write() Options

```c++
float write(int8_t pin, int32_t duty, float frequency, uint8_t resolution, uint32_t phase);
float write(int8_t pin, int32_t duty, float frequency, uint8_t resolution);
float write(int8_t pin, int32_t duty, float frequency);
float write(int8_t pin, int32_t duty);
```

### [Using pwm.write()](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/Using%20pwmWrite.md)

### Basic Syntax

```c++
pwm.write(pin, duty);
```

### Parameters

`pin`: The GPIO pin to write to.  Allowed data types: `int`.
`value`: The duty cycle between 0 (always off) and `pow(2, resolution)` (always on). This function automatically attaches the pin to the first available channel. To avoid conflicts with other code, the chosen pin will not be available if it was previously accessed by other code. Various overload functions are provided (shown above) allowing the user to use only the parameters needed at any time.

### writeFrequency()

```c++
float writeFrequency(int8_t pin, float frequency);
```

Sets the PWM frequency (default 1000Hz) on any PWM pin. Returns the exact hardware frequency used by the timer.

### writeResolution()

```c++
int32_t writeResolution(int8_t pin, uint8_t resolution);
```

The PWM resolution for any PWM pin can be set from 1-bit to 16-bit (default 8-bit). Returns the maximum value that will be always on for the selected resolution: `pow(2, resolution)`

### setPinsStatusDefaults()

```c++
void setPinsStatusDefaults(int32_t duty = 0, float frequency = 1000, uint8_t resolution = 8, uint32_t phase = 0);
```

This function sets the default PWM duty, frequency, resolution and phase for all 8 channels. If called with no arguments, the defaults are as shown above.  This is also the same as the startup defaults, so it's not a requirement to call this function unless new defaults are required.

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

### Notes

- Both timer resolution and PWM frequency should be calculated to get expected results.
-  Refer to [Supported Range of Frequency and Duty Resolution](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution) as a reference. 

```
This Library is licensed under the MIT License
```

