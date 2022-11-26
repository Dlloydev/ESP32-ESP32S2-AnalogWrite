## Using pwm.write()

Using this function is very easy as much is done transparently. Initializing a pin with `pinMode` is not required as attaching the pin to a channel and setting up the timer resource is automatic.  The various parameters available are: 

| Parameter       | Default | Description                                |
| --------------- | ------- | ------------------------------------------ |
| pin             | 255     | no pin attached                            |
| frequency       | 1000    | pwm frequency (Hz)                         |
| resolution      | 8       | bit width                                  |
| duty            | 0       | duty cycle                                 |
| phase           | 0       | phase shift                                |
| returned value: | 0       | returns configured timer frequency (float) |

### Prototypes:

```c++
float write(int8_t pin, int32_t duty, float frequency, uint8_t resolution, uint32_t phase);
float write(int8_t pin, int32_t duty, float frequency, uint8_t resolution);
float write(int8_t pin, int32_t duty, float frequency);
float write(int8_t pin, int32_t duty);
```

### Examples

Beginning of sketch:

```c++
#include <pwmWrite.h>    // include header file
const byte ledPin = 5;   // led connected to pin 5
Pwm pwm = Pwm();         // create an object
```

Syntax: 

```c++
pwm.write()
```

Then in setup() or loop() as desired. Unspecified parameters will be at default:

```c++
pwm.write(ledPin, 127);   // 50% duty, default 1000 Hz, default 8-bit, default 0 phase shift
```

```c++
pwm.write(ledPin, 127, 5000);   // 50% duty, 5000 Hz, default 8-bit, default 0 phase shift
```

```c++
pwm.write(ledPin, 63, 500, 8);   // 25% duty, 500 Hz, 8-bit, default 0 phase shift
```

```c++
pwm.write(ledPin, 255, 500, 10, 255);   // 25% duty, 500 Hz, 10-bit, 25% phase shift
```





