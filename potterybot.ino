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
Adafruit_StepperMotor *horizMotor = AFMS.getStepper(400, 1);
Adafruit_StepperMotor *vertMotor = AFMS.getStepper(400, 2);

//Define Variables
//The array of x,y points that define where we want our arm to move
const float points[5][2] = {{1,2},{2,3},{3,3},{4,4},{5,5}};
//Our Current Position
float currentPos [2] = {0,0};
//The index of the point that we want to get to
int pointIndex = 0;
//The current goal point
float currentPoint[2] = {points[0][0], points[0][1]};
//The maximum pressure
const int maxPressure = 10;
//Max steps per loop
const int maxSteps = 400;
//The tolerance for reaching points
const float pointTolerance = 0.0111;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  horizMotor->setSpeed(10);  // 10 rpm   
  vertMotor->setSpeed(10);  // 10 rpm   
}

void loop() {
Serial.print("Current Pos: ");
Serial.print(currentPos[0]);
Serial.print(",");
Serial.print(currentPos[1]);
Serial.println(" ");
Serial.print("Current Point: ");
Serial.print(currentPoint[0]);
Serial.print(",");
Serial.print(currentPoint[1]);
Serial.println(" ");
//calculate the x,y delta between where we are and where we want to be  
float delta[2] = {currentPoint[0] - currentPos[0],currentPoint[1] - currentPos[1]};
//This is a dummy reading. Once we get the sensor hooked up, we'll read the pressure here.
int pressure = 5;
if (abs(delta[0]) < pointTolerance && abs(delta[1]) < pointTolerance) {
  pointIndex = pointIndex+1;
  currentPoint[0] = points[pointIndex][0];
  currentPoint[1] = points[pointIndex][1];
}else {
  if (pressure<maxPressure) {
     int stepsNeeded[2] = {deltaPosToSteps(delta[0]),deltaPosToSteps(delta[1])};
     if (abs(stepsNeeded[0]) < maxSteps) {
        if(stepsNeeded[0] < 0) {
          Serial.println("I'm moving the horizontal motor backward - almost there");
          //horizMotor->step(abs(stepsNeeded[0]), BACKWARD, DOUBLE); 
        }else{
          Serial.println("I'm moving the horizontal motor forward - almost there");
          //horizMotor->step(abs(stepsNeeded[0]), FORWARD, DOUBLE); 
        }
        currentPos[0] = currentPos[0] + stepsToDeltaPos(stepsNeeded[0]);
     }else {
        if(stepsNeeded[0] < 0) {
          Serial.println("I'm moving the horizontal motor backward");
          //horizMotor->step(maxSteps, BACKWARD, DOUBLE);
          currentPos[0] = currentPos[0] - stepsToDeltaPos(maxSteps); 
        }else{
          Serial.println("I'm moving the horizontal motor forward");
          //horizMotor->step(maxSteps, FORWARD, DOUBLE); 
          currentPos[0] = currentPos[0] + stepsToDeltaPos(maxSteps); 
        } 
     }
     if (abs(stepsNeeded[1]) < maxSteps) {
       if(stepsNeeded[1] < 0) {
         Serial.println("I'm moving the vetical motor backward - almost there");
         //vertMotor->step(abs(stepsNeeded[1]), BACKWARD, DOUBLE); 
       }else{
         Serial.println("I'm moving the vertical motor forward - almost there");
         //horizMotor->step(abs(stepsNeeded[1]), FORWARD, DOUBLE); 
       }
       currentPos[1] = currentPos[1] + stepsToDeltaPos(stepsNeeded[1]);
     }else {
       if(stepsNeeded[0] < 0) {
         Serial.println("I'm moving the horizontal motor backward");
         //horizMotor->step(maxSteps, BACKWARD, DOUBLE);
         currentPos[1] = currentPos[1] - stepsToDeltaPos(maxSteps); 
       }else{
         Serial.println("I'm moving the horizontal motor forward");
         //horizMotor->step(maxSteps, FORWARD, DOUBLE); 
         currentPos[1] = currentPos[1] + stepsToDeltaPos(maxSteps); 
       } 
     }
  }
}
delay(1000);
}


int deltaPosToSteps(float deltaPos){
  //Given the change in position we want, calculate the number of revolutions.
  float deltaPosPerStep = 0.0002775;
  int steps = deltaPos/deltaPosPerStep;
  return steps;
  
}

float stepsToDeltaPos(int steps){
  float deltaPosPerStep = 0.0002775;
  float deltaPos = steps*deltaPosPerStep;
  return deltaPos;
}

