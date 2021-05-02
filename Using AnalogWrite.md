## Using AnalogWrite

Using analogWrite is very easy as much is done transparently. Initializing a pin with pinMode is not required as attaching the pin to a channel and setting up the timer resource is automatic.  The various parameters available are: 

| Parameter       | Default | Description                            |
| --------------- | ------- | -------------------------------------- |
| pin             | -1      | default is no pin attached             |
| frequency       | 980     | pwm frequency (Hz)                     |
| resolution      | 8       | bit width                              |
| value           | 0       | output low                             |
| phase           | 0       | no phase shift                         |
| returned value: | 0       | returns actual timer frequency (float) |

### Prototypes:

```c++
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution, uint32_t phase);
float analogWrite(int8_t pin, int32_t value, float frequency, uint8_t resolution);
float analogWrite(int8_t pin, int32_t value, float frequency);
float analogWrite(int8_t pin, int32_t value);
```

### Examples

Beginning of sketch:

```c++
const int ledPin = 5;   // LED connected to digital pin 5
```



Then in setup() or loop() as desired. Unspecified parameters will be at default:

```c++
analogWrite(ledPin, 127);   // 50% duty, default 980 Hz, default 8-bit, default 0 phase shift
```

```c++
analogWrite(ledPin, 127, 5000);   // 50% duty, 5000 Hz, default 8-bit, default 0 phase shift
```

```c++
analogWrite(ledPin, 63, 500, 8);   // 25% duty, 500 Hz, 8-bit, default 0 phase shift
```

```c++
analogWrite(ledPin, 255, 500, 10, 255);   // 25% duty, 500 Hz, 10-bit, 25% phase shift
```

```c++
analogWrite(DAC1, 127);   // DAC1 pin set at VCC/2 volts 
```

```c++
analogWrite(ledPin, -1);   // detaches ledPin (floating) and timer channel cleared
```



