/*
  Controls three servos with different easing settings
  New position command after normalized output (ye) reaches set value.
  https://wokwi.com/projects/361237697368753153
  by dlloydev, March 2023.

  ⚪ The green servo moves from 0 to 90 deg at 70 deg/s with linear motion.
  ⚪ The orange servo moves from 0 to 180 deg at 140 deg/s with sigmoid motion.
  ⚪ The purple servo moves from 45 to 135 deg at 180 deg/s with steep sigmoid motion.
*/

#include <Servo.h>

const int servoPin1 = 18;
const int servoPin2 = 19;
const int servoPin3 = 21;

// units in degrees per second
double speed1 = 70.0;
double speed2 = 140.0;
double speed3 = 180.0;

// When easing constant (ke) < 1.0, return value is normalized, when 1.0, returns pulse width (μs)
// ke = 0.0 is linear, between 0.0 and 1.0 is tunable sigmoid, 1.0 is normal response
// Normalized Tunable Sigmoid: https://www.desmos.com/calculator/ejkcwglzd1
double ke1 = 0.0;
double ke2 = 0.6;
double ke3 = 0.8;

// go to position (degrees)
uint8_t pos1 = 90;
uint8_t pos2 = 180;
uint8_t pos3 = 135;

float ye1, ye2, ye3;

Servo myservo = Servo();

void setup() {
  Serial.begin(115200);
}

void loop() {

  if (ye1 <= 0.0 ) pos1 = 90;
  else if (ye1 >= 1.0) pos1 = 0;

  if (ye2 <= 0.0 ) pos2 = 180;
  else if (ye2 >= 1.0) pos2 = 0;

  if (ye3 <= 0.0 ) pos3 = 135;
  else if (ye3 >= 1.0) pos3 = 45;

  ye1 = myservo.write(servoPin1, pos1, speed1, ke1);
  ye2 = myservo.write(servoPin2, pos2, speed2, ke2);
  ye3 = myservo.write(servoPin3, pos3, speed3, ke3);
  Serial.print(ye1);
  Serial.print(",");
  Serial.print(ye2);
  Serial.print(",");
  Serial.println(ye3);
  delay(6);
}
