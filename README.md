# ESP32 PWM, Servo, Easing and Tone Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32%20ESP32S2%20AnalogWrite.svg?)](https://www.ardu-badge.com/ESP32%20ESP32S2%20AnalogWrite)  <a href="https://registry.platformio.org/libraries/dlloydev/ESP32 ESP32S2 AnalogWrite"><img src="https://badges.registry.platformio.org/packages/dlloydev/library/ESP32 ESP32S2 AnalogWrite.svg" alt="PlatformIO Registry" /></a>

<details>

<summary><h3>Comparison to Servo Library for Arduino</h3></summary>


- Both libraries use the same header filename: `Servo.h`
- Methods in both libraries have identical names.
- With the Servo Library for Arduino, each servo is instantiated, whereas only one instance is used with the ESP32 ESP32S2 AnalogWrite library to control up to 16 servos. Therefore, the `write()` method in the ESP32 ESP32S2 AnalogWrite library has a pin parameter to select the attached servo.

#### Comparison Table

- Superscript values represent the number of available overload functions .
- With the ESP32 ESP32S2 AnalogWrite library, both `Servo.h` and `pwmWrite.h` have access to all methods. Choose **one** header only that best suits your application. Note that `Servo.h` uses a Servo class that translates method names to match the Servo Library for Arduino. Each header gives full access to the libraries features.

| Library: | [Servo Library for Arduino](https://github.com/arduino-libraries/Servo) | [ESP32 ESP32S2 AnalogWrite](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite) | [ESP32 ESP32S2 AnalogWrite](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite) |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Header   | Servo.h                                                      | Servo.h                                                      | pwmWrite.h                                                   |
| Includes | ServoTimers.h                                                | pwmWrite.h                                                   | driver/ledc.h                                                |
| Methods  | attach()<sup> 2</sup>                                        | attach()<sup> 10</sup>                                       | attachServo()<sup> 10</sup>                                  |
|          | write()                                                      | write()<sup> 2</sup>                                         | writeServo()<sup> 2</sup>                                    |
|          | writeMicroseconds()                                          | writeMicroseconds()                                          | n/a                                                          |
|          | read()                                                       | read()                                                       | read()                                                       |
|          | attached()                                                   | attached()                                                   | attached()                                                   |
|          | detach()                                                     | detach()                                                     | detach()                                                     |
|          | attachedPin()                                                | attachedPin()                                                | attachedPin()                                                |
|          | readMicroseconds()                                           | readMicroseconds()                                           | readMicroseconds()                                           |
|          |                                                              | attachPwm()<sup> 2</sup>                                     | attach()<sup> 2</sup>                                        |
|          |                                                              | attachInvert()<sup> 2</sup>                                  | attachInvert()<sup> 2</sup>                                  |
|          |                                                              | writePwm()<sup> 4</sup>                                      | write()<sup> 4</sup>                                         |
|          |                                                              | detached()                                                   | detached()                                                   |
|          |                                                              | firstFreeCh()                                                | firstFreeCh()                                                |
|          |                                                              | pause()                                                      | pause()                                                      |
|          |                                                              | resume()                                                     | resume()                                                     |
|          |                                                              | printDebug()                                                 | printDebug()                                                 |
|          |                                                              | setFrequency()                                               | setFrequency()                                               |
|          |                                                              | setResolution()                                              | setResolution()                                              |
|          |                                                              | tone()                                                       | tone()                                                       |
|          |                                                              | note()                                                       | note()                                                       |

</details>

![image](https://github.com/Dlloydev/jtag2updi/assets/63488701/8217e847-b427-4b2b-9f39-f941578af63d)

### Description

This library uses the ESP32 Arduino framework's [ledc](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.c) functions and provides up to 16 channels for servos, pwm, leds, buzzers etc. Includes smart GPIO pin management where any pin will not be automatically attached if previously accessed by other code. Includes advanced control methods like timer pause/resume, phase delay using hpoint, inverted pwm and tunable servo easing.

Servo Easing is fully integrated into the servo write and attach functions. Only 2 parameters give complete control over the speed and the easing characteristic of the servo. The method used for easing is a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1) ([reference](https://dhemery.github.io/DHE-Modules/technical/sigmoid/)).



#### Arduino core for the ESP32, ESP32-S2, ESP32-S3 and ESP32-C3

Recommend using the [latest release](https://github.com/espressif/arduino-esp32), however this library works with release 2.0.7 or newer.

<details>

<summary><h3>Servo Easing</h3></summary>

Just 2 easing parameters (speed and easing constant) for unlimited control ...

```c++
myservo.write(servoPin1, pos1, speed1, 0.0);  // move 90 deg at 70 deg/s, linear
myservo.write(servoPin2, pos2, speed2, 0.6);  // mpve 180 deg at 140 deg/s, avg sigmoid
myservo.write(servoPin3, pos3, speed3, 0.8);  // move 90 deg at 180 deg/s, steep sigmoid
```

#### ![ServoEasing](https://user-images.githubusercontent.com/63488701/227943891-87cb7555-fe56-4064-a83a-38b99ad58e1d.gif)

#### Speed Control:

The maximum speed in degrees/sec is derived from the servo's datasheet. For this [SG90 Micro Servo](https://robojax.com/learn/arduino/robojax-servo-sg90_datasheet.pdf) we have  Operating speed: 0.1 s/60 degree. In this case, the maximum value for the speed parameter is 600 deg/sec. When a new servo position value is set, the operating time in milliseconds = degrees to move / speed * 1000.

#### Easing Control:

The easing constant ke controls how the servo moves to the set position by varying the speed. Its effect from linear (ke = 0.0) to maximum steep curve (ke = 0.99).

#### Position Feedback: 

The calculated position of the servo is the returned value "ye" of the writeServo function. The easing position ye is normalized (0.0-1.0) but can slightly over/undershoot this range. The servo has reached its programmed position when ye = 1.0 if the new setting is larger than previous and also when ye = 0.0 if the new position setting is smaller than previous.

#### servoWrite:

After a new servo position is programmed, repeatedly call the servoWrite function with the same parameters until the servo completes its motion (returned value ye = 1.0 or 0.0). The servo responds according to ke and speed. Servo position is incremented after each call. 

</details>

<details>
<summary><h3>Examples</h3></summary>

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351231798778266200)  [Note Explorer ♩ ♪ ♫ ♬](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Note_Explorer/Note_Explorer.ino)   Plays all 96 ledc notes that are available, non blocking

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351175246893548120)  [Note_Player](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Note_Player/Note_Player.ino)   Playing Notes based on sliding pot position, 4th octave, non blocking

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349336125753524820)  [Pwm_3phase_40kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_3phase_40kHz/Pwm_3phase_40kHz.ino)   ESP32 3 Phase PWM Outputs (40kHz, 10-bit)

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334722465700774482)  [Pwm_ESP32_3phase_10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_ESP32_3phase_10kHz/Pwm_ESP32_3phase_10kHz.ino)   ESP32 3 Phase PWM Outputs (10kHz, 10-bit)

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334856585002091092)  [Pwm_ESP32_C3_3phase_10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_ESP32_3phase_10kHz/Pwm_ESP32_3phase_10kHz.ino)   ESP32 C3 3 Phase PWM Outputs (10kHz, 10-bit)

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/334765722024542804)  [Pwm_ESP32_S2_3phase_10kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_ESP32_S2_3phase_10kHz/Pwm_ESP32_S2_3phase_10kHz.ino)   ESP32 S2 3 Phase PWM Outputs (10kHz, 10-bit)

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349978851105833554)  [Pwm_Fade_Servo](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_Fade_Servo/Pwm_Fade_Servo.ino)   ESP32 fading 14 pairs of LEDs and controlling 2 servo motors

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349232255258853970)  [Pwm_Fade16](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_Fade16/Pwm_Fade16.ino)   ESP32 fading 16 pairs of LEDs

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/349322326995632722)  [Pwm_Sync2_300kHz](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Pwm_Sync2_300kHz/Pwm_Sync2_300kHz.ino)   2 synchronized PWM outputs using the same timer (channel pair)

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/364791981216008193)  [Servo_Easing_Interrupt](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Easing_Interrupt/Servo_Easing_Interrupt.ino)   Servo Easing with position feedback and Interrupt control

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/361237697368753153)  [Servo_Easing_Position](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Easing_Position/Servo_Easing_Position.ino)   3 servos with easing and position feedback control

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/350033311963284051)  [Servo Knob](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Knob/Servo_Knob.ino)   Controls servo position by using a potentiometer 

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/355852275661848577)  [Servo_Knob_Six](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Knob_Six/Servo_Knob_Six.ino)   Potentiometer control of 6 servos on an ESP32-C3

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/350037178957431378)  [Servo Sweep](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Sweep/Servo_Sweep.ino)   Sweep a servo motor from 0-180 degrees and back

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351967394028061269)  [Servo_Sweep_Inverted](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Sweep_Inverted/Servo_Sweep_Inverted.ino)   Using inverted PWM mode to sweep a servo motor

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/351978833396630095)  [Servo_Sweep_Speed](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Servo_Sweep_Speed/Servo_Sweep_Speed.ino)  Independent speed control of 2 servos

[![Wokwi_badge](https://user-images.githubusercontent.com/63488701/212449119-a8510897-c860-4545-8c1a-794169547ba1.svg)](https://wokwi.com/projects/352178590336932865)  [Tone_Player](https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite/blob/main/examples/Tone_Player/Tone_Player.ino)   Playing Tones based on sliding pot position

</details>

<details>

<summary><h3>PWM Channel Configuration</h3></summary>

| Board       | PWM Pins                  | PWM, Duty and Phase Channels | Frequency and Resolution Channels |
| ----------- | ------------------------- | ---------------------------- | --------------------------------- |
| ESP32       | 0-19, 21-23, 25-27, 32-39 | 16                           | 8                                 |
| ESP32‑S2/S3 | 0-21, 26, 33-45           | 8                            | 4                                 |
| ESP32‑C3    | 0- 10, 18-21              | 6                            | 3                                 |

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

</details>

## Reference (Servo.h)

### Include and Instantiate

```c++
#include <Servo.h>
Servo myservo = Servo();
```

<details>

<summary><h3>write()</h3></summary>

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
myservo.write(pin, value)
myservo.write(pin, value, speed, ke)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(int)*
- **value**  This value is converted to the pwm duty. See above table for range and units *(double)
- **speed**  This value has units degrees/second (double). For example, if `speed` is set to 100 deg/s and the servo position value is changed from 0 to 180 deg, then the servo will take 1.8 sec (1800 ms) to complete its travel. Its motion (response) will be determined by `ke`,
- **ke**  Servo easing constant for a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1). A `ke` value of 0.0 represents a linear response. As you increase `ke`, this increases the steepness of a sigmoid response. When `ke` is 1.0, normal "instantaneous" servo response is enabled and the speed parameter is ignored.

##### Returns

- If the servo easing constant `ke` is 1.0 (default) then the pwm duty value *(uint32_t)* is returned.
- If  `ke` is less than 1.0, then a normalized double value (0.0 to 1.0) is returned. This represents the programmed servo position from start to stop as it moves over time. When the returned value reaches 0.5, this represents both 50% travel and 50% time duration, no matter what easing constant is set.

</details>

<details>

<summary><h3>writeMicroseconds()</h3></summary>

##### Description

This function calls the write() function above.

**Syntax**

```c++
myservo.writeMicroseconds()
```

</details>

<details>

<summary><h3>read()</h3></summary>

##### Description

Read the current angle of the servo in degrees. The returned value is *float* type which provides improved resolution and takes advantage of the high resolution offered by the timer.

**Syntax**

```c++
myservo.read(pin)
```

##### Parameters

- **pin**  The pin number (int)

##### Returns

- The angle of the servo, from 0 to 180 degrees *(float)*

</details>

<details>

<summary><h3>readMicroseconds()</h3></summary>

##### Description

Reads the timer channel's duty value in microseconds. The minimum limit is 544 μs representing 0 degrees shaft rotation and the  maximum limit is 2400 μs representing 180 degrees shaft rotation. The returned value is *float* type which provides improved resolution and takes advantage of the high resolution offered by the timer.

**Syntax**

```c++
myservo.readMicroseconds(pin)
```

##### Parameters

- **pin**  The pin number (int)

##### Returns

- The channel's duty value converted to microseconds *(float)*

</details>

<details>

<summary><h3>attach()</h3></summary>

##### Description

This function allows auto-attaching a pin to the first available channel if only the pin is specified. To have the pin assigned to a specific channel, use both the pin and channel (ch) parameters. Additionally, there are parameters available for setting the servo timer values for minimum and maximum microseconds. 

**Syntax**

```c++
myservo.attach(pin)                                       // auto attach to 1st free channel
myservo.attach(pin, invert)                               // as above with invert
myservo.attach(pin, ch)                                   // attach to specified channel
myservo.attach(pin, ch, invert)                           // as above with invert
myservo.attach(pin, minUs, maxUs)                         // auto attach to free ch, servo limits
myservo.attach(pin, ch, minUs, maxUs)                     // attach to specified ch, servo limits
myservo.attach(pin, ch, minUs, maxUs, invert)             // as above with invert
myservo.attach(pin, minUs, maxUs, speed, ke)              // attach to free ch, speed, easing const
myservo.attach(pin, ch, minUs, maxUs, speed, ke)          // as above but attaches to specified ch
myservo.attach(pin, ch, minUs, maxUs, speed, ke, invert)  // as above with invert
```

##### Parameters

- **pin**  The pin number *(int)*

- **ch**  This optional parameter is used to attach the pin to a specific channel *(int)*)

- **minUs**  Minimum timer width in microseconds *(int)*

- **maxUs**  Maximum timer width in microseconds *(int)*

- **speed**  This servo easing parameter has units degrees/second (double). For example, if `speed` is set to 100 deg/s and the servo position value is changed from 0 to 180 deg, then the servo will take 1.8 sec (1800 ms) to complete its travel. Its motion (response) will be determined by `ke`,

- **ke**  Servo easing constant for a [Normalized Tunable Sigmoid](https://www.desmos.com/calculator/ejkcwglzd1). A `ke` value of 0.0 represents a linear response. As you increase `ke`, this increases the steepness of a sigmoid response. When `ke` is 1.0, normal "instantaneous" servo response is enabled and the speed parameter is ignored.

- **invert**  Inverts the PWM output. Allows using a simpler driver for higher voltage servo control. Only one NPN transistor or N-Channel MOSFET needed. No additional latency added as found with software inversion because the inverted pulse remains at the start of the refresh period rather than being flipped to the end of the refresh period  *(bool)*.

  [Servo_Sweep_Inverted](https://wokwi.com/projects/351967394028061269)

  ![image](https://user-images.githubusercontent.com/63488701/236273265-0cdf2dca-78b8-4afd-8924-1f263c7cde80.png)

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*

</details>

<details>

<summary><h3>attachPwm()</h3></summary>

##### Description

This function allows auto-attaching a pin to the first available channel if only the pin is specified. To have the pin assigned to a specific channel, use both the pin and channel (ch) parameters. 

**Syntax**

```c++
myservo.attach(pin)       // auto attach to 1st free channel
myservo.attach(pin, ch)   // attach to specified channel
```

##### Parameters

- **pin**  The pin number *(int)*

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*

</details>

<details>

<summary><h3>attached()</h3></summary>

##### Description

This function checks the pin status and if attached, returns the channel number. 

**Syntax**

```c++
myservo.attached(pin)
```

##### Parameters

- **pin**  The pin number *(int)*

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*

</details>

<details>

<summary><h3>attachInvert()</h3></summary>

##### Description

This function allows auto-attaching a pin to the first available channel if only the pin is specified. To have the pin assigned to a specific channel, use both the pin and channel (ch) parameters. The pwm output will be inverted. The duty value represents the low period.

**Syntax**

```c++
myservo.attachInvert(pin);      // attach pin to next free channel with inverted pwm
myservo.attachInvert(pin, ch);  // attach to specified ch with inverted pwm
```

##### Parameters

- **pin**  The pin number *(int)*
- **ch**  This optional parameter is used to attach the pin to a specific channel *(int)*

##### Returns

- If not a valid pin, 254 *(uint8_t)*
- free channels exist, 253 *(uint8_t)*
- If attached, the channel number (0-15) *(uint8_t)*
- If not attached, 255 *(uint8_t)*

</details>

<details>


<summary><h3>attachedPin()</h3></summary>

##### Description

This function returns the pin that's attached to the specified channel.

**Syntax**

```c++
myservo.attachedPin(ch)
```

##### Parameters

- **pin**  The pin number *(int)*

##### Returns

- If attached, the pin number *(uint8_t)*
- If the channel is free, 255 *(uint8_t)*

</details>


<details>

<summary><h3>writePwm()</h3></summary>

##### Description

This function writes the duty and optionally the frequency, resolution and phase parameters. If necessary, the pin will be automatically attached to the first available pwm channel. To avoid conflicts with other code, the pin will not be attached if previously accessed.

##### Syntax

```c++
myservo.writePwm(pin, duty)
myservo.writePwm(pin, duty, frequency)
myservo.writePwm(pin, duty, frequency, resolution)
myservo.writePwm(pin, duty, frequency, resolution, phase)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(int)*
- **duty**  This sets the pwm duty. The range is 0 to (2**resolution) - 1 *(uint32_t)*
- **frequency**  The pwm timer frequency (Hz). The frequency and resolution limits are interdependent *(uint32_t)*. For more details, see [Supported Range of Frequency and Duty Resolutions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html#ledc-api-supported-range-frequency-duty-resolution).
- **resolution**  The bit resolution of the pwm duty *(uint8_t)*
- **phase**  This is also referred to as the **hpoint** value, which is the timer/counter value that the pwm output turns on. The useable range is the same as for the duty parameter. This can be used to phase shift the output or for synchronization. When the phase parameter is used, the pwm output will initiate in a paused state to allow synchronization *(uint32_t)*

##### Returns

The set frequency *(float)*

</details>

<details>

<summary><h3>detachPin()</h3></summary>

##### Description

This function removes control of the pin from the specified PWM channel.  Also, the channel defaults are applied.

**Syntax**

```c++
myservo.detachPin(pin)
```

##### Parameters

- **pin**  The pin number *(int)*

##### Returns

- nothing

</details>

<details>

<summary><h3>pause()</h3></summary>

##### Description

This function is used internally by the write() function when the phase parameter is used to allow synchronization of multiple pwm signals. 

If this function is manually called, any channel(s) that get configured will have their PWM output paused.  Then calling `resume()` will start all newly configured channels at the same time. Note that this approach limits the maximum pwm frequency to about 10kHz or some pulses or glitches might occur during channel configuration.

**Syntax**

```c++
myservo.pause()
```

##### Parameters

- none.

##### Returns

- nothing

</details>

<details>

<summary><h3>resume()</h3></summary>

##### Description

This function is used to start the pwm outputs of all channels to synchronize (align) the signals. Note that there will be a consistent delay between the startup of each timer which can be corrected by using the `write()` function's phase parameter.

**Syntax**

```c++
myservo.resume()
```

##### Parameters

- none.

##### Returns

- nothing

</details>

<details>

<summary><h3>setFrequency()</h3></summary>

##### Description

Sets the PWM frequency on any PWM pin.

**Syntax**

```c++
myservo.setFrequency(pin, frequency)
```

##### Parameters

- **pin**  The pin number  *(int)* If the pin is detached (free) and there's a free channel available, the pin will be attached to the first free channel that's found *(int)*
- **frequency**  The frequency in Hz. The default is 1000 Hz *(uint32_t)*

##### Returns

- The frequency set by the timer hardware *(float)*

</details>

<details>

<summary><h3>setResolution()</h3></summary>

##### Description

Sets the PWM resolution for any PWM pin.

**Syntax**

```c++
myservo.setResolution(pin, resolution)
```

##### Parameters

- **pin**  The pin number  *(int)* If the pin is detached (free) and there's a free channel available, the pin will be attached to the first free channel that's found *(int)*
- **resolution**  The PWM resolution can be set from 1-bit to 16-bit, default is 8-bit *(uint8_t)*

##### Returns

- The set resolution reported by the pin channel *(uint8_t)*

</details>

<details>

<summary><h3>tone()</h3></summary>

##### Description:

This function generates a square wave of the specified frequency (and 50% duty  cycle and 8-bit resolution) on a pin. There will be no output (no tone) if the duration isn't specified or equals 0. The duration in milliseconds has range 0-65535 where 0 is off and 65535 is always on. The last parameter (interval)  specifies the pause time before the next call to tone becomes ready. The pin can be connected to a piezo buzzer or other speaker to play tones.

**Channel Pairing**

The frequency and resolution values are shared by each channel pair. When the tone pin is attached, the next lower or higher channel on the same timer gets updated with the same frequency and resolution values as appropriate.

**Attaching to free Channel**

This process is automatic - the tone pin will be attached to the next free channel. If you need to assign the tone pin to a specific channel, then call the `attach()`method first.

##### Syntax

```c++
myservo.tone(pin, frequency, duration)
myservo.tone(pin, frequency, duration, interval)
```

##### Parameters

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(int)*
- **frequency**  The tone frequency (Hz) with range 1-65535 *(uint16_t)*.
- **duration**  The duration in milliseconds with range 0-65535 *(uint16_t)*, where 0 is off (default) and 65535 is always on.
- **interval**  This optional parameter specifies the pause time in milliseconds before the next call to tone becomes ready. *(uint16_t)*, range 0-65535, default = 0.

##### Returns

- nothing

</details>

<details>

<summary><h3>note()</h3></summary>

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

- **pin**  The pin number which (if necessary) will be attached to the next free channel *(int)*
- **note**  The type is defined in [esp32-hal-ledc.h](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-ledc.h) *(note_t)*.
- **octave**  There are 8 octaves available, 1 to 8 *(uint8_t)* 
- **duration**  The duration in milliseconds with range 0-65535 *(uint16_t)*, where 0 is off (default) and 65535 is always on.
- **interval**  This parameter specifies the pause time in milliseconds before the next call to tone becomes ready. *(uint16_t)*, range 0-65535, default = 0.

##### Returns

- nothing

</details>

<details>

<summary><h3>printDebug()</h3></summary>

##### Description

This function prints the available PWM pins to choose from and a formatted output showing the PWM pins that are in use (attached) and the channels that are unassigned (255).

**Syntax**

```c++
myservo.printDebug()
```

##### Parameters (optional)

- none

##### Returns

- serial report on serial monitor

![image](https://user-images.githubusercontent.com/63488701/229374511-de75b97d-f91f-44d0-b103-0ca858d16727.png)

</details>

```
This Library is licensed under the MIT License
```
