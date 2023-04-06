# ESP32 PWM, Servo, Easing and Tone Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)  <a href="https://registry.platformio.org/libraries/dlloydev/ESP32 ESP32S2 AnalogWrite"><img src="https://badges.registry.platformio.org/packages/dlloydev/library/ESP32 ESP32S2 AnalogWrite.svg" alt="PlatformIO Registry" /></a>

![image](https://user-images.githubusercontent.com/63488701/207152696-7162de8e-bea7-4353-9ae3-682bc40c4e68.png)

### Description

This library wraps the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 16 PWM channels.  Includes smart GPIO pin management where any pin will not be automatically configured if it has been previously accessed by other code. Some advanced control features are auto or manual pin to channel attaching and timer pause and resume methods.

PWM can be inverted, phase shifted and asynchronously aligned with the timing of other pwm channels. 

Servo Easing is fully integrated into the servo write and attach functions. Only 2 parameters give complete control over the speed and the easing characteristic of the servo. The method used for easing is a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1) ([reference](https://dhemery.github.io/DHE-Modules/technical/sigmoid/)). An optionally inverted servo pwm feature allows using a simple NPN or N-Channel MOSFET driver for the servo's control signal.

#### Servo Easing

Just 2 easing parameters (speed and easing constant) for unlimited control ...

```c++
pwm.writeServo(servoPin1, pos1, speed1, 0.0);  // move 90 deg, 70 deg/s, linear
pwm.writeServo(servoPin2, pos2, speed2, 0.6);  // mpve 180 deg, 140 deg/s, avg sigmoid
pwm.writeServo(servoPin3, pos3, speed3, 0.8);  // move 90 deg, 180 deg/s, steep sigmoid
```

#### ![ServoEasing](https://user-images.githubusercontent.com/63488701/227943891-87cb7555-fe56-4064-a83a-38b99ad58e1d.gif)



##### **Examples:**

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/360276061783595009)  [Servo_Easing_Time](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Easing_Time/Servo_Easing_Time.ino)   3 servos with different easing constants and timed position control

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/361237697368753153)  [Servo_Easing_Position](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Easing_Position/Servo_Easing_Position.ino)   3 servos with different easing constants and position feedback control

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/355852275661848577)  [ESP32_C3_6_Servo_Knob](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_C3_6_Servo_Knob/ESP32_C3_6_Servo_Knob.ino)   Potentiometer control of 6 servos on an ESP32-C3

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349232255258853970)  [16 PWM Fade](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_Fade16/ESP32_Fade16.ino)   ESP32 fading 16 pairs of LEDs

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349978851105833554)  [14 PWM Fade 2 Servo](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_Fade_Servo/ESP32_Fade_Servo.ino)   ESP32 fading 14 pairs of LEDs and controlling 2 servo motors

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/350037178957431378)  [Servo Sweep](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Sweep/Servo_Sweep.ino)   Sweep a servo motor from 0-180 degrees and back

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351967394028061269)  [Servo_Sweep_Inverted](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Sweep_Inverted/Servo_Sweep_Inverted.ino)   Using inverted PWM mode to sweep a servo motor

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351978833396630095)  [Dual Servo Sweep with Independent Speed Control](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Dual_Servo_Sweep_Speed/Dual_Servo_Sweep_Speed.ino) 

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/350033311963284051)  [Servo Knob](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Knob/Servo_Knob.ino)   Controls servo position by using a potentiometer 

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351231798778266200)  [Note Explorer ♩ ♪ ♫ ♬](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_Note_Explorer/ESP32_Note_Explorer.ino)   Plays all 96 ledc notes that are available, non blocking

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351175246893548120)  [Playing Notes](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/playingNotes/playingNotes.ino)   Playing Notes based on sliding pot position, 4th octave, non blocking

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/352178590336932865)  [Playing Tones](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/playingTones/playingTones.ino)   Playing Tones based on sliding pot position, 4Hz to 4095Hz, non blocking

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349322326995632722)  [2 Sync 300kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_Sync2_300kHz/ESP32_Sync2_300kHz.ino)   2 synchronized PWM outputs using the same timer (channel pair)

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349336125753524820)  [ESP32_3-Phase 40kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_3phase_40kHz/ESP32_3phase_40kHz.ino)   ESP32 3 Phase PWM Outputs (40kHz, 10-bit)

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334722465700774482)  [ESP32_3-Phase 10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_3phase_10kHz/ESP32_3phase_10kHz.ino)   ESP32 3 Phase PWM Outputs (10kHz, 10-bit)

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334765722024542804)  [ESP32_S2_3-Phase_10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_S2_3phase_10kHz/ESP32_S2_3phase_10kHz.ino)   ESP32 S2 3 Phase PWM Outputs (10kHz, 10-bit)

- [![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334856585002091092)  [ESP32_C3_3-Phase_10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/ESP32_C3_3phase_10kHz/ESP32_C3_3phase_10kHz.ino)   ESP32 C3 3 Phase PWM Outputs (10kHz, 10-bit)

  

| Board       | PWM Pins                  | PWM, Duty and Phase Channels | Frequency and Resolution Channels |
| ----------- | ------------------------- | ---------------------------- | --------------------------------- |
| ESP32       | 0-19, 21-23, 25-27, 32-39 | 16                           | 8                                 |
| ESP32‑S2/S3 | 0-21, 26, 33-45           | 8                            | 4                                 |
| ESP32‑C3    | 0- 10, 18-21              | 6                            | 3                                 |

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

This process is automatic - the servo pin will be attached to the next free channel. If you need to assign the servo pin(s) to specific channels or to set the minimum or maximum microsecond values, then call the `attach()`method first.

##### Syntax

```c++
pwm.writeServo(pin, value)
pwm.writeServo(pin, value, speed, ke)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(uint8_t)*
- **value**  This value is converted to the pwm duty. See above table for range and units *(float)
- **speed**  This value has units degrees/second (float). For example, if `speed` is set to 100 deg/s and the servo position value is changed from 0 to 180 deg, then the servo will take 1.8 sec (1800 ms) to complete its travel. Its motion (response) will be determined by `ke`,
- **ke**  Servo easing constant for a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1). A `ke` value of 0.0 represents a linear response. As you increase `ke`, this increases the steepness of a sigmoid response. When `ke` is 1.0, normal "instantaneous" servo response is enabled and the speed parameter is ignored.

##### Returns

- If the servo easing constant `ke` is 1.0 (default) then the pwm duty value *(uint32_t)* is returned.
- If  `ke` is less than 1.0, then a normalized float value (0.0 to 1.0) is returned. This represents the programmed servo position from start to stop as it moves over time. When the returned value reaches 0.5, this represents both 50% travel and 50% time duration, no matter what easing constant is set.



### tone()

##### Description:

This function generates a square wave of the specified frequency (and 50% duty  cycle and 8-bit resolution) on a pin. There will be no output (no tone) if the duration isn't specified or equals 0. The duration in milliseconds has range 0-65535 where 0 is off and 65535 is always on. The last parameter (interval)  specifies the pause time before the next call to tone becomes ready. The pin can be connected to a piezo buzzer or other speaker to play tones.

**Channel Pairing**

The frequency and resolution values are shared by each channel pair. When the tone pin is attached, the next lower or higher channel on the same timer gets updated with the same frequency and resolution values as appropriate.

**Attaching to free Channel**

This process is automatic - the tone pin will be attached to the next free channel. If you need to assign the tone pin to a specific channel, then call the `attach()`method first.

##### Syntax

```c++
pwm.tone(pin, frequency, duration)
pwm.tone(pin, frequency, duration, interval)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(uint8_t)*
- **frequency**  The tone frequency (Hz) with range 1-65535 *(uint16_t)*.
- **duration**  The duration in milliseconds with range 0-65535 *(uint16_t)*, where 0 is off (default) and 65535 is always on.
- **interval**  This optional parameter specifies the pause time in milliseconds before the next call to tone becomes ready. *(uint16_t)*, range 0-65535, default = 0.

##### Returns

- nothing



### note()

##### Description:

This function generates a square wave of the specified frequency (and 50% duty  cycle and 8-bit resolution) on a pin. There will be no output (no tone) if the duration isn't specified or equals 0. The duration in milliseconds has range 0-65535 where 0 is off and 65535 is always on. The last parameter (interval)  specifies the pause time before the next call to note becomes ready. The pin can be connected to a piezo buzzer or other speaker to play notes.

**Channel Pairing**

The frequency and resolution values are shared by each channel pair. When the note pin is attached, the next lower or higher channel on the same timer gets updated with the same frequency and resolution values as appropriate.

**Attaching to free Channel**

This process is automatic - the note pin will be attached to the next free channel. If you need to assign the tone pin to a specific channel, then call the `attach()`method first.

##### Syntax

```c++
pwm.note(pin, note, octave, duration, interval)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(uint8_t)*
- **note**  The type is defined in [esp32-hal-ledc.h](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.h) *(note_t)*.
- **octave**  There are 8 octaves available, 1 to 8 *(uint8_t)* 
- **duration**  The duration in milliseconds with range 0-65535 *(uint16_t)*, where 0 is off (default) and 65535 is always on.
- **interval**  This parameter specifies the pause time in milliseconds before the next call to tone becomes ready. *(uint16_t)*, range 0-65535, default = 0.

##### Returns

- nothing



### read()

##### Description

Read the current angle of the servo in degrees. The returned value is *float* type which provides improved resolution and takes advantage of the high resolution offered by the timer.

**Syntax**

```c++
read(pin)
```

##### Parameters

- **pin**  The pin number *(uint8_t)

##### Returns

- The angle of the servo, from 0 to 180 degrees *(float)*



### readMicroseconds()

##### Description

Reads the timer channel's duty value in microseconds. The minimum limit is 544 μs representing 0 degrees shaft rotation and the  maximum limit is 2400 μs representing 180 degrees shaft rotation. The returned value is *float* type which provides improved resolution and takes advantage of the high resolution offered by the timer.

**Syntax**

```c++
readMicroseconds(pin)
```

##### Parameters

- **pin**  The pin number *(uint8_t)

##### Returns

- The channel's duty value converted to microseconds *(float)*



### attach()

##### Description

This function allows auto-attaching a pin to the first available channel if only the pin is specified. To have the pin assigned to a specific channel, use both the pin and channel (ch) parameters. Additionally, there are parameters available for setting the servo timer values for minimum and maximum microseconds. 

**Syntax**

```c++
attach(pin)                                       // auto attach to 1st free channel
attach(pin, ch)                                   // attach to specified channel 
attach(pin, minUs, maxUs)                         // auto attach to free ch with servo limits
attach(pin, ch, minUs, maxUs)                     // attach to specified ch with servo limits
attach(pin, minUs, maxUs, speed, ke)              // attach to free ch with speed and easing constant
attach(pin, ch, minUs, maxUs, speed, ke)          // as above but attaches to specified channel
attach(pin, ch, minUs, maxUs, speed, ke, invert)  // as above with invert  
```

##### Parameters

- **pin**  The pin number *(uint8_t)*

- **ch**  This optional parameter is used to attach the pin to a specific channel *(uint8_t)*)

- **minUs**  Minimum timer width in microseconds *(uint16_t)

- **maxUs**  Maximum timer width in microseconds *(uint16_t)*

- **speed**  This servo easing parameter has units degrees/second (float). For example, if `speed` is set to 100 deg/s and the servo position value is changed from 0 to 180 deg, then the servo will take 1.8 sec (1800 ms) to complete its travel. Its motion (response) will be determined by `ke`,

- **ke**  Servo easing constant for a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1). A `ke` value of 0.0 represents a linear response. As you increase `ke`, this increases the steepness of a sigmoid response. When `ke` is 1.0, normal "instantaneous" servo response is enabled and the speed parameter is ignored.

- **invert**  Inverts the PWM output. Allows using a simpler driver for higher voltage servo control. Only one NPN transistor or N-Channel MOSFET needed. No additional latency added as found with software inversion because the inverted pulse remains at the start of the refresh period rather than being flipped to the end of the refresh period  *(bool)*.

  [Servo_Sweep_Inverted](https://wokwi.com/projects/351967394028061269)

  ![image](https://user-images.githubusercontent.com/63488701/229374262-460e878e-81f1-4398-8ea5-60b02026a4cf.png)

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*



### attachInvert()

##### Description

This function allows auto-attaching a pin to the first available channel if only the pin is specified. To have the pin assigned to a specific channel, use both the pin and channel (ch) parameters. The pwm output will be inverted. The duty value represents the low period.

**Syntax**

```c++
attachInvert(pin);      // attach pin to next free channel with inverted pwm
attachInvert(pin, ch);  // attach to specified ch with inverted pwm
```

##### Parameters

- **pin**  The pin number *(uint8_t)*
- **ch**  This optional parameter is used to attach the pin to a specific channel *(uint8_t)*)

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*



### attached()

##### Description

This function checks the pin status and if attached, returns the channel number. 

**Syntax**

```c++
attached(pin)
```

##### Parameters

- **pin**  The pin number *(uint8_t)

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*



### attachedPin()

##### Description

This function returns the pin that's attached to the specified channel.

**Syntax**

```c++
attachedPin(ch)
```

##### Parameters

- **pin**  The pin number *(uint8_t)

##### Returns

- If attached, the pin number *(uint8_t)*
- If the channel is free, 255 *(uint8_t)*



### detachPin()

##### Description

This function removes control of the pin from the specified PWM channel.  Also, the channel defaults are applied.

**Syntax**

```c++
pwm.detachPin(pin)
```

##### Parameters

- **pin**  The pin number *(uint8_t)*

##### Returns

- nothing



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



### printDebug()

##### Description

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are unassigned (255).

**Syntax**

```c++
pwm.printDebug()
```

##### Parameters (optional)

- none

##### Returns

- serial report on serial monitor

![image](https://user-images.githubusercontent.com/63488701/229374511-de75b97d-f91f-44d0-b103-0ca858d16727.png)

```
This Library is licensed under the MIT License
```

