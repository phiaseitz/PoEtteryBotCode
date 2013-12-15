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
//Define Variables
//The array of x,y points that define where we want our arm to move
const float points[5] = {1,2,3,4,5};
//Our Current Position Purely dependent on Break Beam Sensors. 
float currentPos = 0;
//The index of the point that we want to get to
int pointIndex = 0;
//The current goal point
float currentPoint = points[0];
//The maximum pressure
const int maxPressure = 10;
//Max steps per loop
const int maxSteps = 400;
//The tolerance for reaching points
const float pointTolerance = 0.0111;
//How much does the position change inbetween each part of the breakbeam sensor
const float deltaPosPerBreak= 0.111/1;
//The pin for the break beam sensor
const int horizontalBreakBeamPin = 8;
//The previous break beam reading
int previousHBreakBeam = 0;


void setup() {
  delay(3000);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
 
 //Horizontal Stepper
  horizontalStepper.setMaxSpeed(motorSpeed);
  horizontalStepper.setSpeed(motorSpeed);
  
  //Break Beam Sensor
  pinMode(horizontalBreakBeamPin, INPUT);
}

void loop() {


//This is a dummy reading. Once we get the sensor hooked up, we'll read the pressure here.
int pressure = 5;
//Check to see if the breakbeam sensor is high and increment position
int horizontalBreakBeam = digitalRead(horizontalBreakBeamPin);
if ((horizontalBreakBeam == 1) &&(horizontalBreakBeam =! previousHBreakBeam)) {
  currentPos = currentPos + deltaPosPerBreak;
}

//calculate the x,y delta between where we are and where we want to be  
float delta = currentPoint - currentPos;

if (abs(delta) < pointTolerance)  {
  pointIndex = pointIndex+1;
  currentPoint = points[pointIndex];
  Serial.print("Moving on to target position: ");
  Serial.println(currentPoint);
}else {
  if (pressure<maxPressure) {
     int stepsNeeded = deltaPosToSteps(delta);
     if (abs(stepsNeeded) < maxSteps) {
        if(stepsNeeded < 0) {
          horizontalStepper.setSpeed(-halfSpeed);
        }else{
          horizontalStepper.setSpeed(halfSpeed);
        }
     }else {
        if(stepsNeeded < 0) {
          horizontalStepper.setSpeed(-motorSpeed);
        }else{
          horizontalStepper.setSpeed(motorSpeed);
        } 
     }
  } else {
    horizontalStepper.setSpeed(0);
  } 
}
horizontalStepper.run();
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

