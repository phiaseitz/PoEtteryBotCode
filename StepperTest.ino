/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 400 steps per revolution (0.9 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(400, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(50);  // 10 rpm   
}

void loop() {
  
  /*Serial.println("Double coil steps");
  myMotor->step(400, FORWARD, DOUBLE); 
  //myMotor->step(400, BACKWARD, DOUBLE);*/
  
  int testing = deltaPosToSteps(0.111);
  Serial.println(testing);
  
  
}

int deltaPosToSteps(float deltaPos){
  //Given the change in position we want, calculate the number of revolutions.
  int stepsPerRev = 400;
  float deltaPosPerRev = 0.111;
  float deltaPosPerStep = 0.0002775;
  int steps = deltaPos/deltaPosPerStep;
  return steps;
  
}

