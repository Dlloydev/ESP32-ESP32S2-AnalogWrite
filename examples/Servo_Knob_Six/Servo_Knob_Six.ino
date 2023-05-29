/*
  Independent potentiometer control of 6 servos on an ESP32-C3
  https://wokwi.com/projects/355852275661848577
  by dlloydev, February 2023.
*/

#include <Servo.h>

Servo myservo = Servo();

const int potPin1 = 0, servoPin1 = 6;
const int potPin2 = 1, servoPin2 = 7;
const int potPin3 = 2, servoPin3 = 8;
const int potPin4 = 3, servoPin4 = 9;
const int potPin5 = 4, servoPin5 = 18;
const int potPin6 = 5, servoPin6 = 19;

void setup() {
}

void loop() {

  int val1 = analogRead(potPin1);       // read the pot value (0-4095)
  val1 = map(val1, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin1, val1);      // set the servo position (degrees)

  int val2 = analogRead(potPin2);       // read the pot value (0-4095)
  val2 = map(val2, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin2, val2);      // set the servo position (degrees)

  int val3 = analogRead(potPin3);       // read the pot value (0-4095)
  val3 = map(val3, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin3, val3);      // set the servo position (degrees)

  int val4 = analogRead(potPin4);       // read the pot value (0-4095)
  val4 = map(val4, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin4, val4);      // set the servo position (degrees)

  int val5 = analogRead(potPin5);       // read the pot value (0-4095)
  val5 = map(val5, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin5, val5);      // set the servo position (degrees)

  int val6 = analogRead(potPin6);       // read the pot value (0-4095)
  val6 = map(val6, 700, 3395, 0, 180);  // align pot pointer to servo arm
  myservo.write(servoPin6, val6);      // set the servo position (degrees)

  delay(10);
}
