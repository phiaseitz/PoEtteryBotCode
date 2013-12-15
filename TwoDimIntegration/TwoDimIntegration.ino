/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <AccelStepper.h>



//Defining Pins for Motors and Sensors
//Horizontal Stepper motor
const int horizontalMotorDirPin = 2; //digital pin 2
const int horizontalMotorStepPin = 3; //digital pin 3
//Vertical Stepper motor
const int verticalMotorDirPin = 4; //digital pin 4
const int verticalMotorStepPin = 5; //digitial pin 5
//Horizontal Break Beam Sensor
const int horizontalBreakBeamPin = 8; //digital pin 8
//Vertical Break Beam Sensor
const int verticalBreakBeamPin = 9; //digital pin 9
//Pressure sensor pin
const int pressureSensorPin = A0; //analog pin A0

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper horizontalStepper(1, horizontalMotorStepPin, horizontalMotorDirPin); 
AccelStepper verticalStepper(1, verticalMotorStepPin, verticalMotorDirPin); 

//Set up constants
//Our motor has 400 steps per revolution
//maximum steps per second
const int motorSpeed = 3000; 
//The array of x,y points that define where we want our arm to move
const float points[5][2] = {{0,-2},{-1.5,-2},{-2,-1},{-2.75,0}, {0,0}};
//How much does the position change inbetween each part of the breakbeam sensor
const float deltaPosPerBreak= 0.111/1;
//The tolerance for reaching points
const float pointTolerance = deltaPosPerBreak;
//The maximum pressure
const int maxPressure = 10;

//Define Variables

//Our Current Position Purely dependent on Break Beam Sensors. 
float currentPos [2] = {0,0};
//The index of the point that we want to get to
int pointIndex = 0;
//The current goal point
float currentPoint[2] = {points[0][0], points[0][1]};
//The previous break beam reading
int previousHBreakBeam = 0;
//The previous break beam reading
int previousVBreakBeam = 0;



void setup() {
  //delay so that we can open the serial port
  delay(3000);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
 
 //Horizontal Stepper
  horizontalStepper.setMaxSpeed(2*motorSpeed);
  horizontalStepper.setSpeed(motorSpeed);
  //Vertical Stepper
  verticalStepper.setMaxSpeed(2*motorSpeed);
  verticalStepper.setSpeed(motorSpeed);
  
  
  //Break Beam Sensors
  //horizontal
  pinMode(horizontalBreakBeamPin, INPUT);
  //vertical
  pinMode(verticalBreakBeamPin, INPUT);
  //Pressure Sensor
  pinMode(pressureSensorPin, INPUT);
}

void loop() {


//This is a dummy reading. Once we get the sensor hooked up, we'll read the pressure here.
int pressure = 5;
//int pressure = analogRead(pressureSensorPin);

//calculate the x,y delta between where we are and where we want to be  
float delta[2] = {0,0};
delta[0] = currentPoint[0] - currentPos[0];
delta[1] = currentPoint[1] - currentPos[1];

//Check to see if the breakbeam sensor is high and increment horizontal position
int horizontalBreakBeam = digitalRead(horizontalBreakBeamPin);
if ((horizontalBreakBeam == 1) &&(horizontalBreakBeam =! previousHBreakBeam)) {
  if (delta[0] > 0) {
    currentPos[0] = currentPos[0] + deltaPosPerBreak;
  } else {
    currentPos[0] = currentPos[0] - deltaPosPerBreak;
  }
}

//Check to see if the breakbeam sensor is high and increment vertical position
int verticalBreakBeam = digitalRead(verticalBreakBeamPin);
if ((verticalBreakBeam == 1) &&(verticalBreakBeam =! previousVBreakBeam)) {
  if (delta[1] >0) {
    currentPos[1] = currentPos[1] + deltaPosPerBreak;
  }else {
    currentPos[1] = currentPos[1] - deltaPosPerBreak;
  }
}



//Are we close enough to the target point
if (abs(delta[0]) <= pointTolerance && abs(delta[1]) <= pointTolerance)  {
  //Increment the target point by one
  pointIndex = pointIndex+1;
  //Update our new target position
  currentPoint[0] = points[pointIndex][0];
  currentPoint[1] = points[pointIndex][1];
  Serial.print("Moving on to target position: ");
  Serial.print(currentPoint[0]);
  Serial.print(",");
  Serial.println(currentPoint[1]);
//We aren't there yet
}else {
  //Is the pressure okay?
  if (pressure<maxPressure) {
         //Are we vertically there?
     if(abs(delta[1]) <= pointTolerance) {
        verticalStepper.setSpeed(0);
     //Move down
     }else if(delta[1] < 0) {
       verticalStepper.setSpeed(motorSpeed);
     //Move up
     }else{
       verticalStepper.setSpeed(-motorSpeed);
     }
     
     //Are we horizontally there?
     if(abs(delta[0]) <= pointTolerance) {
         horizontalStepper.setSpeed(0);
     //Move backward
     }else if(delta[0] < 0) {
       horizontalStepper.setSpeed(motorSpeed);
     //Move forward
     }else{
          verticalStepper.setSpeed(motorSpeed);
     }
  //The pressure is too high
  } else {
    //Stop everything
    horizontalStepper.setSpeed(0);
    verticalStepper.setSpeed(0);
  } 
}
horizontalStepper.run();
verticalStepper.run();
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

