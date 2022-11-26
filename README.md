# pwmWrite

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)  <a href="https://registry.platformio.org/libraries/dlloydev/ESP32 ESP32S2 AnalogWrite"><img src="https://badges.registry.platformio.org/packages/dlloydev/library/ESP32 ESP32S2 AnalogWrite.svg" alt="PlatformIO Registry" /></a>

![image](https://user-images.githubusercontent.com/63488701/174445314-c7945015-f295-4cba-917c-cc4ead8d534a.png)



#### *Wokwi [ESP32 16 pwm fade](https://wokwi.com/projects/349232255258853970) example*

#### *Wokwi [3 phase pwm 40kHz](https://wokwi.com/projects/349336125753524820) example*

#### *Wokwi fast and synchronized pwm examples ... [2 signals 300kHz](https://wokwi.com/projects/349322326995632722) ... [8 signals 20kHz](https://wokwi.com/projects/349319723103552084)*



### Description

This library wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 16 PWM channels.  A unique feature is the pin management where any pin will not be configured if it has been previously accessed by other code. If required, the user can also manually assign a pin to any specific channel. The pwm write function can assign a pin, contol PWM duty, frequency, resolution and phase shift all from one function. Timer control functions and examples are provided to demonstrate much improved synchronization of multiple PWM waveforms at high frequencies.

| Board    | PWM Pins                          | PWM, Duty and Phase Channels | Frequency and Resolution Channels |
| -------- | --------------------------------- | ---------------------------- | --------------------------------- |
| ESP32    | 2, 4, 5, 12-19, 21-23, 27, 32, 33 | 16                           | 8                                 |
| ESP32‑S2 | 1- 14, 21, 33-42, 45              | 8                            | 4                                 |
| ESP32‑C3 | 0- 9, 18, 19                      | 6                            | 3                                 |

### PWM Channel Configuration

Frequency and resolution values are shared by each channel pair. For example, channels 0 and 1 share the same frequency and resolution values. When any channel gets configured, the next lower or higher channel gets updated with the same frequency and resolution values as appropriate.

| PWM Channel | Speed Mode | Timer | Frequency | Resolution | Duty | Phase |
| :---------: | ---------- | ----- | --------- | ---------- | ---- | ----- |
|      0      | 0          | 0     | 1         | 1          | 1    | 1     |
|      1      | 0          | 0     | 1         | 1          | 2    | 2     |
|      2      | 0          | 1     | 2         | 2          | 3    | 3     |
|      3      | 0          | 1     | 2         | 2          | 4    | 4     |
|      4      | 0          | 2     | 3         | 3          | 5    | 5     |
|      5      | 0          | 2     | 3         | 3          | 6    | 6     |
|      6      | 0          | 3     | 4         | 4          | 7    | 7     |
|      7      | 0          | 3     | 4         | 4          | 8    | 8     |
|      8      | 1          | 0     | 5         | 5          | 9    | 9     |
|      9      | 1          | 0     | 5         | 5          | 10   | 10    |
|     10      | 1          | 1     | 6         | 6          | 11   | 11    |
|     11      | 1          | 1     | 6         | 6          | 12   | 12    |
|     12      | 1          | 2     | 7         | 7          | 13   | 13    |
|     13      | 1          | 2     | 7         | 7          | 14   | 14    |
|     14      | 1          | 3     | 8         | 8          | 15   | 15    |
|     15      | 1          | 3     | 8         | 8          | 16   | 16    |



## write()

##### Description

This function automatically attaches the pin to the first available pwm channel. To avoid conflicts with other code, the chosen pin will not be attached if it was previously accessed by other code. Various overload functions are provided (shown below) allowing the user to use only the parameters needed at any time.

```c++
float write(uint8_t pin, uint32_t duty);
float write(uint8_t pin, uint32_t duty, uint32_t frequency);
float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution);
float write(uint8_t pin, uint32_t duty, uint32_t frequency, uint8_t resolution, uint32_t phase);
```

##### Syntax

`pwm.write(pin, duty);`

##### Parameters

- **pin**  The gpio pin number (pwm output)
- **duty**  This sets the pwm duty. The range is 0 to (2**resolution) - 1 
- **frequency**  The pwm timer frequency (Hz). The frequency range and the duty resolution are interdependent. The higher the PWM frequency, the lower the duty resolution which is available, and  vice versa. For more details, see Section [Supported Range of Frequency and Duty Resolutions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution).
- **resolution**  The bit resolution of the pwm duty.
- **phase**  This is also referred to as the **hpoint** value, which is the timer/counter value that the pwm output turns on. The useable range is the same as for the duty parameter. This can be used to phase shift the output or for synchronization by adjusting the starting point of the pwm signal. When the phase parameter is used, the pwm signal will initiate in a paused state so that multiple pwm outputs can be setup first then later resumed at the same time to aid in timer synchronization.

##### Returns

The set frequency *(float)*

#### [Using pwm.write()](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/Using%20pwmWrite.md)



### attachPin()

```c++
void attachPin(uint8_t pin, uint8_t ch);
```

This function accepts two arguments. The first is the pin (GPIO) that will output the pwm signal and the second is the pwm channel that it will be assigned to.



### detachPin()

```c++
void detachPin(uint8_t pin, uint8_t ch);
```

This function removes control of the pin from the associated PWM channel



### pause()

```c++
void pause(uint8_t ch);
```

This function is used internally by the write() function when the phase parameter is used to allow synchronization of multiple pwm signals.



### resume()

```c++
void resume(uint8_t ch);
```

This function is also used to resume or start a pwm signal. Calling this for multiple channels allows synchronization of multiple pwm signals.



### getChannel()

```c++
uint8_t getChannel(uint8_t pin);
```

This function returns the pin's associated PWM channel.



### setFrequency()

```c++
float setFrequency(uint8_t pin, uint32_t frequency);
```

Sets the PWM frequency on any PWM pin. Returns the exact hardware frequency used by the timer.



### setResolution()

```c++
uint32_t setResolution(uint8_t pin, uint8_t resolution);
```

The PWM resolution for any PWM pin can be set from 1-bit to 16-bit (default 8-bit). Returns the maximum value.



### setPinsStatusDefaults()

```c++
void setPinsStatusDefaults(uint32_t duty = 0, uint32_t frequency = 1000, uint8_t resolution = 8, uint32_t phase = 0);
```

This function sets the default PWM duty, frequency, resolution and phase for all 8 channels. If called with no arguments, the defaults are as shown above.  This is also the same as the startup defaults, so it's not a requirement to call this function unless new defaults are required.



### printPinsStatus()

```c++
void printPinsStatus();
```

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are unassigned (255). Resource management is handled automatically and transparently. There is no requirement to have any function in the setup loop. PWM resolution and frequency can be changed at any time in the main loop. The available PWM pins are also printed.



### Notes

- Both timer resolution and PWM frequency should be calculated to get expected results.
-  Refer to [Supported Range of Frequency and Duty Resolution](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution) as a reference. 

```
This Library is licensed under the MIT License
```

