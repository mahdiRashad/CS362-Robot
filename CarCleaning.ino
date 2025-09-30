#include <Servo.h>

Servo myservo;  // create servo object

void setup() {
  myservo.attach(3);  // attach to digital pin 9 (PWM capable)
}

void loop() {
  // Full speed clockwise
  myservo.write(180);  
}
