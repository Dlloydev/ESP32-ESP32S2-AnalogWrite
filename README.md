# ESP32 PWM and SERVO Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)  <a href="https://registry.platformio.org/libraries/dlloydev/ESP32 ESP32S2 AnalogWrite"><img src="https://badges.registry.platformio.org/packages/dlloydev/library/ESP32 ESP32S2 AnalogWrite.svg" alt="PlatformIO Registry" /></a>

![image](https://user-images.githubusercontent.com/63488701/174445314-c7945015-f295-4cba-917c-cc4ead8d534a.png)



### Description

This library wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 16 PWM channels.  A unique feature is the pin management where any pin will not be automatically configured if it has been previously accessed by other code. If required, the user can also manually assign a pin to any specific channel. The pwm write function can attach a pin, control PWM duty, frequency, resolution and phase shift all from one function. Timer pause and resume functions are provided for improved synchronization of multiple PWM waveforms at higher frequencies.

**Simulation Examples:**

-  [16 pwm fade](https://wokwi.com/projects/349232255258853970)
-  [14 pwm fade 2 servo](https://wokwi.com/projects/349978851105833554)
-  [Servo Knob](https://wokwi.com/projects/350033311963284051)
-  [Servo Sweep](https://wokwi.com/projects/350037178957431378)
-  [3 phase 40kHz](https://wokwi.com/projects/349336125753524820)
-  [2 sync 300kHz](https://wokwi.com/projects/349322326995632722)
-  [8 sync 20kHz](https://wokwi.com/projects/349319723103552084)

| Board    | PWM Pins                          | PWM, Duty and Phase Channels | Frequency and Resolution Channels |
| -------- | --------------------------------- | ---------------------------- | --------------------------------- |
| ESP32    | 2, 4, 5, 12-19, 21-23, 27, 32, 33 | 16                           | 8                                 |
| ESP32‑S2 | 1- 14, 21, 33-42, 45              | 8                            | 4                                 |
| ESP32‑C3 | 0- 9, 18, 19                      | 6                            | 3                                 |

### PWM Channel Configuration

Frequency and resolution values are shared by each channel pair thats on the same timer. When any channel gets configured, the next lower or higher channel gets updated with the same frequency and resolution values as appropriate.

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



### write()

##### Description

This function writes the duty and optionally the frequency, resolution and phase parameters. If necessary, the pin will be automatically attached to the first available pwm channel. To avoid conflicts with other code, the pin will not be attached if previously accessed.

##### Syntax

```c++
pwm.write(pin, duty)
pwm.write(pin, duty, frequency)
pwm.write(pin, duty, frequency, resolution)
pwm.write(pin, duty, frequency, resolution, phase)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(uint8_t)*
- **duty**  This sets the pwm duty. The range is 0 to (2**resolution) - 1 *(uint32_t)*
- **frequency**  The pwm timer frequency (Hz). The frequency and resolution limits are interdependent *(uint32_t)*. For more details, see [Supported Range of Frequency and Duty Resolutions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution).
- **resolution**  The bit resolution of the pwm duty *(uint8_t)*
- **phase**  This is also referred to as the **hpoint** value, which is the timer/counter value that the pwm output turns on. The useable range is the same as for the duty parameter. This can be used to phase shift the output or for synchronization. When the phase parameter is used, the pwm output will initiate in a paused state to allow synchronization *(uint32_t)*

##### Returns

The set frequency *(float)*

#### [Using write()](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/Using%20pwmWrite.md)



### setServo()

##### Description:

This function sets the minimum, default and maxumum servo values in microseconds. Accepted range is 500 to 2500 microseconds.

##### Syntax

```c++
pwm.setServo(channel, minUs, defUs, maxUs)
```

##### Parameters

- **channel**  The pwm channel *(uint8_t)*
- **minUs, defUs, maxUs**  These are the minimum, default and maximum limit in microseconds (uint16_t)

##### Returns

Nothing



### writeServo()

##### Description:

This function accepts a value of type *float* that's processed to an unsigned duty value that takes full advantage of the servo channel's set resolution. If using a standard positional servo, this will set the angle of the shaft in degrees with range 0-180.  If using a continuous rotation servo, this will set the speed where the limits 0 and 180 are full speed in each direction and where the mid range (90) is no movement.

| Entered Value *(float)*  | Coerced Value *(float)*  | Units        |
| :----------------------- | :----------------------- | :----------- |
| < 0                      | 0                        | degrees      |
| 0-180                    | 0-180                    | degrees      |
| > 180 AND < 500          | 180                      | degrees      |
| ≥ 500 AND < servoMinUs   | servoMinUs               | microseconds |
| servoMinUs to servoMaxUs | servoMinUs to servoMaxUs | microseconds |
| > servoMaxUs             | servoMaxUs               | microseconds |

**Timer Width (resolution)**

When using this function, the timer width (resolution) will be14 bit if the target architecture is ESP32C3. For ESP32/S2/S3, the maximum bit width will be 20, which allows setting any width from14 to 20.

**Servo Frequency**

The allowed range for servo frequency is 40 to 900 Hz. Any saved or entered frequency that's out of this range, will be set and saved as 50Hz.

**Channel Pairing**

The frequency and resolution values are shared by each channel pair. When any channel gets configured, the next lower or higher channel on the same timer gets updated with the same frequency and resolution values as appropriate.

**Attaching to free Channel**

This process is automatic - the servo pin will be attached to the next free channel. If you need to organize pins on specific channels, then use call the `attachPin()`method first. 

##### Syntax

```c++
pwm.writeServo(pin, value)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(uint8_t)*
- **value**  This value is converted to the pwm duty. See above table for range and units *(float)

##### Returns

The pwm duty value *(uint32_t)*



### attachPin()

##### Description

This function accepts two arguments, the pin number and optionally the pwm channel number. This function is normally not required as it is automatically called by other functions if necessary. However, if the user needs to manually organize the pin to channel assignments, using this function with the channel parameter will be necessary.

**Syntax**

```c++
pwm.attachPin(pin)
pwm.attachPin(pin, channel);
```

##### Parameters

- **pin**  The pin number *(uint8_t)*
- **channel**  This optional parameter can be used to attach the pin to a specific channel (if free).

##### Returns

- If successful, the channel number (0-15) *(uint8_t)*
- If no vacancy, 253 *(uint8_t)*



### detachPin()

##### Description

This function removes control of the pin from the specified PWM channel.  Also, the channel defaults are applied.

**Syntax**

```c++
pwm.detachPin(pin, channel)
```

##### Parameters

- **pin**  The pin number *(uint8_t)*
- **channel**  The channel to reset to default status with no pin assigned (free) *(uint8_t)*

##### Returns

- nothing



### getPinStatus()

##### Description

This function returns the channel number that's assigned to the specified pin. Otherwise, returns if the pin is free or in use and/or no free channel.

**Syntax**

```c++
pwm.getPinStatus(pin)
```

##### Parameters

- **pin**  The pin number.

##### Returns

- If attached, returns the channel number (0-15) *(uint8_t)*
- If the pin is free, returns 255 *(uint8_t)*
- If the pin isn't available (used by other code or there's no free channel), returns 253 *(uint8_t)*



### getPinOnChannel()

##### Description

This function returns the pin number that's attached to the specified PWM channel.

**Syntax**

```c++
pwm.getPinOnChannel(channel)
```

##### Parameters

- **channel**  The channel number.

##### Returns

- If the channel has a pin attached, returns the pin number *(uint8_t)*
- If the channel is free, returns 255 *(uint8_t)*



### pause()

##### Description

This function is used internally by the write() function when the phase parameter is used to allow synchronization of multiple pwm signals. 

If this function is manually called, any channel(s) that get configured will have their PWM output paused.  Then calling `resume()` will start all newly configured channels at the same time. Note that this approach limits the maximum pwm frequency to about 10kHz or some pulses or glitches might occur during channel configuration.

**Syntax**

```c++
pwm.pause()
```

##### Parameters

- none.

##### Returns

- nothing



### resume()

##### Description

This function is used to start the pwm outputs of all channels to synchronize (align) the signals. Note that there will be a consistent delay between the startup of each timer which can be corrected by using the `write()` function's phase parameter.

**Syntax**

```c++
pwm.resume()
```

##### Parameters

- none.

##### Returns

- nothing



### setFrequency()

##### Description

Sets the PWM frequency on any PWM pin.

**Syntax**

```c++
pwm.setFrequency(pin, frequency)
```

##### Parameters

- **pin**  The pin number  (uint8_t) If the pin is detached (free) and there's a free channel available, the pin will be attached to the first free channel that's found *(uint8_t)*
- **frequency**  The frequency in Hz. The default is 1000 Hz *(uint32_t)*

##### Returns

- The frequency set by the timer hardware *(float)*



### setResolution()

##### Description

Sets the PWM resolution for any PWM pin.

**Syntax**

```c++
pwm.setResolution(pin, resolution)
```

##### Parameters

- **pin**  The pin number  (uint8_t) If the pin is detached (free) and there's a free channel available, the pin will be attached to the first free channel that's found *(uint8_t)*
- **resolution**  The PWM resolution can be set from 1-bit to 16-bit, default is 8-bit *(uint8_t)*

##### Returns

- The set resolution reported by the pin channel *(uint8_t)*



### setConfigDefaults()

##### Description

This function sets the default PWM duty, frequency, resolution and phase values for all channels. If called with no arguments, the values are the same as the startup defaults. It's not a requirement to call this function unless new defaults are needed.

**Syntax**

```c++
pwm.setConfigDefaults()
pwm.setConfigDefaults(duty, frequency, resolution, phase)
```

##### Parameters (optional)

- **duty**  This sets the pwm duty. The range is 0 to (2^resolution) - 1, default is 0 *(uint32_t)*
- **frequency**  The frequency in Hz. The default is 1000 Hz *(uint32_t)**
- **resolution**  The PWM resolution can be set from 1-bit to 16-bit, default is 8-bit *(uint8_t)*
- **phase**  The phase range is the same as for the duty parameter, default is 0 *(uint32_t)*

##### Returns

- nothing



### printConfig()

##### Description

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are unassigned (255).

**Syntax**

```c++
pwm.printConfig()
```

##### Parameters (optional)

- none

##### Returns

- serial text report on serial monitor

![image](https://user-images.githubusercontent.com/63488701/205149918-2410ed2e-d376-4b6c-bdda-1ff890ad421e.png)



```
This Library is licensed under the MIT License
```

