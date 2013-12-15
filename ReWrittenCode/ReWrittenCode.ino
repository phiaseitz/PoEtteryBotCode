//This is an example of how you would control 1 stepper

#include <AccelStepper.h>
//All the Motor Things
int motorSpeed = 1000; //maximum steps per second (about 3rps / at 16 microsteps)
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

//Points (Reference from breadboard side)
//Positions
//Positive is up and to the right

//Speeds
//Negative (Up, Right)
//Positive (Down, Left)

const float = deltaPosBreak = 0.111/1;
const float target[2] = {3,0};
float pos[2] = {0,0};




void setup(){
  Serial.begin(9600); 
  //Motors
  stepper1.setMaxSpeed(motorSpeed);
  stepper1.setSpeed(motorSpeed);
  
  //Negative Speed is Up
  stepper2.setMaxSpeed(motorSpeed);
  stepper2.setSpeed(-motorSpeed);
  
  //Break Beam Sensors
  //horizontal
  pinMode(horizontalBreakBeamPin, INPUT);
  //vertical
  pinMode(verticalBreakBeamPin, INPUT);
}

void loop(){
  int hBreakBeamReading = digitalRead(horizontalBreakBeamPin);
  int vBreakBeamReading = digitalRead(verticalBreakBeamPin);
  if (hBreakBeamReading == 1 && lastHBreakBeam == 0) {  
     pos[0] = pos[0] +  deltaPosPerBreak;
  }
  if (vBreakBeamReading == 1 && lastVBreakBeam == 0) {
     pos[1] = pos[1] +  deltaPosPerBreak;
  }

  float delta[2] = {target[0] - pos[0], target[1] - pos[1]};
  
  int hMotorSpeed = delta[0] * motorSpeed;
  int vMotorSpeed = delta[1] * motorSpeed;
  
 stepper1.setSpeed(hMotorSpeed);
 stepper2.setSpeed(-vMotorSpeed);
  
  
  stepper1.runSpeed();
  stepper2.runSpeed();
  
  lastHBreakBeam = hBreakBeamReading;
  lastVBreakBeam = vBreakBeamReading;
}
