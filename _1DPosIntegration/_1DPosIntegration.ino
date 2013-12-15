/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <AccelStepper.h>

int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int halfSpeed = motorSpeed/2;

int horizontalMotorDirPin = 2; //digital pin 2
int horizontalMotorStepPin = 3; //digital pin 3

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper horizontalStepper(1, horizontalMotorStepPin, horizontalMotorDirPin); 
AccelStepper verticalStepper(1, verticalMotorStepPin, verticalMotorDirPin); 
//Define Variables
//The array of x,y points that define where we want our arm to move
const float points[5] = {1,2,3,4,,5};
//Our Current Position Purely dependent on Break Beam Sensors. 
float currentPos = 0
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
//How much does the position change inbetween each part of the breakbeam sensor
const float deltaPosPerBreak= 0.111/1;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
 
 //Horizontal Stepper
  horizontalStepper.setMaxSpeed(motorSpeed);
  horizontalStepper.setSpeed(motorSpeed);
  
  verticalStepper.setMaxSpeed(motorSpeed);
  verticalStepper.setSpeed(motorSpeed);
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

//This is a dummy reading. Once we get the sensor hooked up, we'll read the pressure here.
int pressure = 5;
//Check to see if the breakbeam sensor is high and increment position
int horizontalBreakBeam = 0;
if (horizontalBreakBeam > 0) {
  currentPos[0] = currentPos[0] + deltaPosPerBreak;
}

int verticalBreakBeam = 0;
if (verticalBreakBeam > 0) {
  currentPos[0] = currentPos[0] + deltaPosPerBreak;
}

//calculate the x,y delta between where we are and where we want to be  
float delta[2] = {currentPoint[0] - currentPos[0] , currentPoint[1] - currentPos[1]};

if ((abs(delta[0]) < pointTolerance) && (abs(delta[1]) < pointTolerance)) {
  pointIndex = pointIndex+1;
  currentPoint[0] = points[pointIndex][0];
  currentPoint[1] = points[pointIndex][1];
}else {
  if (pressure<maxPressure) {
     int stepsNeeded[2] = {deltaPosToSteps(delta[0]),deltaPosToSteps(delta[1])};
     if (abs(stepsNeeded[0]) < maxSteps) {
        if(stepsNeeded[0] < 0) {
          Serial.println("I'm moving the horizontal motor backward - almost there");
          horizontalStepper.setSpeed(-halfSpeed);
        }else{
          Serial.println("I'm moving the horizontal motor forward - almost there");
          horizontalStepper.setSpeed(halfSpeed);
        }
        currentPos[0] = currentPos[0] + stepsToDeltaPos(stepsNeeded[0]);
     }else {
        if(stepsNeeded[0] < 0) {
          Serial.println("I'm moving the horizontal motor backward");
          horizontalStepper.setSpeed(-motorSpeed);
          currentPos[0] = currentPos[0] - stepsToDeltaPos(maxSteps); 
        }else{
          Serial.println("I'm moving the horizontal motor forward");
          horizontalStepper.setSpeed(motorSpeed);
          currentPos[0] = currentPos[0] + stepsToDeltaPos(maxSteps); 
        } 
     }
     if (abs(stepsNeeded[1]) < maxSteps) {
       if(stepsNeeded[1] < 0) {
         Serial.println("I'm moving the vetical motor backward - almost there");
         verticalStepper.setSpeed(-halfSpeed);
       }else{
         Serial.println("I'm moving the vertical motor forward - almost there");
         verticalStepper.setSpeed(halfSpeed);
       }
       currentPos[1] = currentPos[1] + stepsToDeltaPos(stepsNeeded[1]);
     }else {
       if(stepsNeeded[0] < 0) {
         Serial.println("I'm moving the horizontal motor backward");
         verticalStepper.setSpeed(-motorSpeed);
         currentPos[1] = currentPos[1] - stepsToDeltaPos(maxSteps); 
       }else{
         Serial.println("I'm moving the vertical motor forward");
         verticalStepper.setSpeed(motorSpeed);
         currentPos[1] = currentPos[1] + stepsToDeltaPos(maxSteps); 
       } 
     }
  } else {
    horizontalStepper.setSpeed(0);
    verticalStepper.setSpeed(0);
  } 
}
horizontalStepper.run();
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

