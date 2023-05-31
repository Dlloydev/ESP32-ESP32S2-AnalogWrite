/*
  Servo Easing with feedback based on position and Interrupt controlled sampling
  by David Lloyd, May 2023.
  Simulation: https://wokwi.com/projects/364791981216008193

  Speed Control:
  The maximum speed in degrees/sec is derived from the servo's datasheet. For this SG90
  Micro Servo (https://robojax.com/learn/arduino/robojax-servo-sg90_datasheet.pdf) we have
  Operating speed: 0.1 s/60 degree. Therefore, maximum speed = 600 deg/sec
  When a new servo position value is set, the operating time in milliseconds is:
  degrees to move / speed * 1000.

  Easing Control:
  The easing constant ke controls how the servo moves to the set position by varying the speed.
  Its effect from linear (ke = 0.0) to maximum steep curve (ke = 0.99) can be seen here:
  Normalized Tunable Sigmoid: https://www.desmos.com/calculator/ejkcwglzd1

  Position Feedback:
  The calculated position of the servo is the returned value "ye" of the writeServo function.
  The easing position ye is normalized (0.0-1.0) but can slightly over/undershoot this range.
  The servo has reached its programmed position when ye = 1.0 if the new setting is larger than
  previous and also when ye = 0.0 if the new position setting is smaller than previous.

  servoWrite:
  When a new servo position is programmed, the servoWrite function is repeatedly called with the same
  parameters until the servo completes its motion (returned value ye = 1.0 or 0.0). The servo responds
  on its own according to ke and speed. Stepping of position is not required.

  Sample Period:
  The sample period is controlled by timer interrupt. By default, this is 20 ms (50 ye samples/sec).
  Interrupt reference: https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/

  Blocking delays in  loop:
  The servo motion is unaffected by blocking delays in the loop.
  The delay of 50 ms was added to control the plotter sample period.
*/

#include <Servo.h>

const int servoPin = 8;
volatile double ke = 0.0;      // easing curve
volatile double speed = 100;   // speed control (degrees/second)
volatile float pos = 90;      // servo position (degrees)
volatile float ye;            // calculated servo position (normalized)

hw_timer_t *Timer0 = NULL;

Servo myservo = Servo();

void IRAM_ATTR Timer0_ISR() {
  ye = myservo.write(servoPin, pos, speed, ke);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0, &Timer0_ISR, true);
  timerAlarmWrite(Timer0, 20000, true);  // 20 ms
  myservo.write(servoPin, pos, speed, ke);  // attach servo
  timerAlarmEnable(Timer0);

  // slowly go to 0 degrees, linear easing
  pos = 0.0;
  speed = 30;
  ke = 0.0;
  ye = myservo.write(servoPin, pos, speed, ke);
  while (ye > 0);

  // setup faster speed and new easing curve
  speed = 100;
  ke = 0.7;
  ye = myservo.write(servoPin, pos, speed, ke);
}

void loop() {
  if (ye > 0.99) pos = 0;
  else if (ye < 0.01) pos = 180;

  Serial.print("pos: " );
  Serial.print(pos / 180); // normalized (0-1 value = 0-180 degrees)
  Serial.print("  ye: " );
  Serial.println(ye);
  delay(50);  // only 20 datapoints/sec will still give a smooth simulated plot
}
