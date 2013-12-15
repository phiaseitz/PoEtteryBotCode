//This is an example of how you would control 1 stepper

#include <AccelStepper.h>

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



void setup(){
  stepper1.setMaxSpeed(motorSpeed);
  stepper1.setSpeed(-motorSpeed);
  
  stepper2.setMaxSpeed(motorSpeed);
  stepper2.setSpeed(-motorSpeed);
  
  //stepper.moveTo(32000); //move 32000 steps (should be 10 rev)
}

void loop(){
/*  
  //if stepper is at desired location
  if (stepper.distanceToGo() == 0){
    //go the other way the same amount of steps
    //so if current position is 400 steps out, go position -400
    stepper.moveTo(-stepper.currentPosition()); 
  }
  
*/
 
  //these must be called as often as possible to ensure smooth operation
  //any delay will cause jerky motion
  stepper1.runSpeed();
  //stepper2.runSpeed();
}
