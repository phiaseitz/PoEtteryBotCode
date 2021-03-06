//This is an example of how you would control 1 stepper

#include <AccelStepper.h>
//All the Motor Things
int motorSpeed = 3000; //maximum steps per second (about 3rps / at 16 microsteps)
//int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 2; //digital pin 2
int motorStepPin = 3; //digital pin 3

int motorDirPin2 = 4; //digital pin 2
int motorStepPin2 = 5; //digital pin 3

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper stepper1(1, motorStepPin, motorDirPin); 
AccelStepper stepper2(1, motorStepPin2, motorDirPin2); 

//Break Beam Sensors
//Horizontal Break Beam Sensor
const int horizontalBreakBeamPin = 8; //digital pin 8
//Vertical Break Beam Sensor
const int verticalBreakBeamPin = 7; //digital pin 7
//The last reading from the horizontal break beam sensor
int lastHBreakBeam = 0;
//The last reading from the vertical break beam sensor
int lastVBreakBeam = 0;

//Pressure Sensor
const int maxPressure = 1000;

//Points (Reference from breadboard side)
//Positions
//Positive is up and to the right

//Speeds
//Negative (Up, Right)
//Positive (Down, Left)

const float deltaPosBreak = 0.111/1;
const float targetPoints[4][2] = {{0,-1},{1.15, -1},{3,0}, {0,0}};
int targetIndex = 0;
int finished = 0;
float target[2] = {targetPoints[targetIndex][0], targetPoints[targetIndex][1]};
float pos[2] = {0,0};




void setup(){
  Serial.begin(9600); 
  //Motors
  stepper1.setMaxSpeed(5000);
  stepper1.setSpeed(0);
  
  //Negative Speed is Up
  stepper2.setMaxSpeed(5000);
  stepper2.setSpeed(0);
  
  //Break Beam Sensors
  //horizontal
  pinMode(horizontalBreakBeamPin, INPUT);
  //vertical
  pinMode(verticalBreakBeamPin, INPUT);
}

void loop(){
  float delta[2] = {target[0] - pos[0], target[1] - pos[1]};
  int hBreakBeamReading = digitalRead(horizontalBreakBeamPin);
  int vBreakBeamReading = digitalRead(verticalBreakBeamPin);
  int pressure = 5;
  if (hBreakBeamReading == 1 && lastHBreakBeam == 0) {
    //Serial.println("h");
    if (delta[0] > 0) {   
      pos[0] = pos[0] +  deltaPosBreak;
    } else {
      pos[0] = pos[0] -  deltaPosBreak;
    }
  }
  if (vBreakBeamReading == 1 && lastVBreakBeam == 0) {
    //Serial.println("v");
    if (delta[1] > 0) {   
      pos[1] = pos[1] +  deltaPosBreak;
    } else {
      pos[1] = pos[1] -  deltaPosBreak;
    }
  }
  //Are we close enough to the target point
  if (abs(delta[0]) <= 2*deltaPosBreak && abs(delta[1]) <= 2*deltaPosBreak)  {
    //Serial.println("t");
    //Increment the target point by one
    targetIndex = targetIndex+1;
    if (targetIndex == 5) {
      finished = 1;
    }
    //Update our new target position
    target[0] = targetPoints[targetIndex][0];
    target[1] = targetPoints[targetIndex][1];

//We aren't there yet
}else{
  if ((pressure < maxPressure) && (finished == 0)) {
    int hMotorSpeed = delta[0] * motorSpeed;
    int vMotorSpeed = delta[1] * motorSpeed;
    
    if (abs(delta[0]) < 2*deltaPosBreak) {
      hMotorSpeed = 0;
    }
    
    if (abs(delta[1]) < 2*deltaPosBreak) {
      vMotorSpeed = 0;
    }
   stepper1.setSpeed(hMotorSpeed);
   stepper2.setSpeed(-vMotorSpeed);
   
  } else{
    stepper1.setSpeed(0);
    stepper2.setSpeed(0);
  }
}  
  
  stepper1.runSpeed();
  stepper2.runSpeed();
  
  lastHBreakBeam = hBreakBeamReading;
  lastVBreakBeam = vBreakBeamReading;
}

