/*
Code for Zumo with 4 NiMH batteries and two 75:1 HP micro metal gearmotors.
May need adjust timing and speed
parameters in GridMovement.h.

Jumpers on the front sensor array must be installed in order to connect pin 4 to DN4 and pin 20 to DN2.

After the gyro calibration is done, press button A to start the
line sensor calibration.  The robot will turn left 90 degrees,
return to center, turn right 90 degrees, and then return
approximately to center.  If the angles are incorrect or the
robot turns in circles, then something is probably wrong with the
gyro and you should power off and try again.

The robot's position might shift during calibration.  If none of
the inner three line sensors can see the line, you should reposition it to center the sensors on the
line.
 */

#include <Wire.h>
#include <Zumo32U4.h>
#include "GridMovement.h"
#include "TurnSensor.h"

Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
L3G gyro;
Zumo32U4ProximitySensors proxSensors;


/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Task 0: choose a driving speed for the robot, from 0 to 400. You may need to change this as you complete tasks
int forwardSpeed = 300;

// Holds data read from the proximity sensors
int maxLeftValue;
int maxRightValue;

// A sensor's reading must be greater than or equal to this
// threshold in order for the program to consider that sensor as
// seeing an object.
int proxSensorThreshold = 5;
/*--------------- CONSTANTS FOR OPERATION ---------------*/


void setup()
{
  buttonA.waitForButton();
  proxSensors.initFrontSensor();
  gridMovementSetup();
  delay(500);
}

void loop()
{   
	// Update the speeds of the motors to stay on the centre of the line
	updateMotorSpeeds();

	// While driving, check for any dead ends or interesections
    if(deadEnd() || intersection())
    {
		// Drive stright forward to get to the center of the
		// intersection and check for exits to the left, right, and
		// straight ahead.
		bool foundLeft, foundStraight, foundRight;
		driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);

   // If at T-intersection, we might be at the civilian room so check for civilians
    if(foundLeft&&foundRight&&!foundStraight){
		  if(civilianDetected()){
        rescueCivilian(); // After rescuing the civilian, stop the robot and wait for a button push to restart
        motors.setSpeeds(0,0);
        buzzer.playFrequency(440, 200, 15);
        buttonA.waitForButton(); 
		  }
   }
		// Choose a direction to turn.
		char dir = selectTurn(foundLeft, foundStraight, foundRight);

		// Make the turn.
		turn(dir);
    }
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/
void updateMotorSpeeds()
{  
   // Get the position of the line.
  int position = readSensors();

  // Our "error" is how far we are away from the center of the
  // line, which corresponds to position 2000.
  int error = position - 2000; 
  
  // Take a proportion of the error
  int speedDifference = error / 4;
    
  // Get individual motor speeds.
  int leftSpeed = forwardSpeed + speedDifference;
  int rightSpeed = forwardSpeed - speedDifference;

  // Constrain our motor speeds to be between 0 and forwardSpeed.
  leftSpeed = constrain(leftSpeed, 0, forwardSpeed);
  rightSpeed = constrain(rightSpeed, 0, forwardSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}

char selectTurn(bool foundLeft, bool foundStraight, bool foundRight)
{
  // Prioritizes left hand turns
  if(foundLeft) { return 'L'; }
  else if(foundStraight) { return 'S'; }
  else if(foundRight) { return 'R'; }
  else { return 'B'; }
}

// This function returns TRUE if the robot detects a civilian
bool civilianDetected()
{
  // Scan Left and Right to detect any civilians in a 90 degree range 
  maxLeftValue = scanLeft();
  maxRightValue = scanRight();

  // If a sensor detects something above the threshold then it must be a civilian 
  return maxLeftValue >= proxSensorThreshold || maxRightValue >= proxSensorThreshold;
}

// This function faces towards the civilian, then slowly drives forward, pushing it until it is out of the room. 
void rescueCivilian()
{
  // Read data from left and right proximity sensors
  proxSensors.read();
  int leftValue = proxSensors.countsFrontWithLeftLeds();
  int rightValue = proxSensors.countsFrontWithRightLeds();

  // Civilian is on the left side of the robot, so turn left.
  if (maxLeftValue >= proxSensorThreshold){
    motors.setSpeeds(-100, 100);
    while(leftValue<maxLeftValue){ // Keep turning till left proximity sensor reads the maximum value
      proxSensors.read();
      leftValue = proxSensors.countsFrontWithLeftLeds();
      rightValue = proxSensors.countsFrontWithRightLeds();
    }
    while(leftValue!=rightValue){ // Keep turning till left and right proximity sensor values are equal
      proxSensors.read();
      leftValue = proxSensors.countsFrontWithLeftLeds();
      rightValue = proxSensors.countsFrontWithRightLeds();
    }
    motors.setSpeeds(-100, 100);
    delay(100); // Turn a bit further to center the robot 

    
  } else { // Nothing to left so civilian must be on the right side of the robot, turn right.
    motors.setSpeeds(100, -100);
    while(rightValue<maxRightValue){ // Keep turning till right proximity sensor reads the maximum value
      proxSensors.read();
      leftValue = proxSensors.countsFrontWithLeftLeds();
      rightValue = proxSensors.countsFrontWithRightLeds();
    }
    while(leftValue!=rightValue){ // Keep turning till left and right proximity sensor values are equal
      proxSensors.read();
      leftValue = proxSensors.countsFrontWithLeftLeds();
      rightValue = proxSensors.countsFrontWithRightLeds();
    }
    motors.setSpeeds(100, -100);
    delay(100);
  }
    // stop for 0.5 seconds
    motors.setSpeeds(0,0);
    delay(500);

    // drive straight towards civilian till robot reaches the line
    readSensors();
    while(!(aboveLine(1)&&aboveLine(2)&&aboveLine(3))){
      readSensors();
      motors.setSpeeds(150,150);
    }
}
/*------------------------------------ END FUNCTIONS TO FILL IN ------------------------------------*/








/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
// Takes calibrated readings of the lines sensors and stores them
// in lineSensorValues.  Also returns an estimation of the line
// position.
uint16_t readSensors()
{
  return lineSensors.readLine(lineSensorValues);
}

// Returns true if the sensor is seeing a line.
bool aboveLine(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}

// This function rotates the robot left by 45 degrees and back to the center while scanning for objects.
// Returns the maximum value found by the left proximity sensor
int scanLeft()
{
  turnSensorReset();
  proxSensors.read();
  
  int maxLeftValue = proxSensors.countsFrontWithLeftLeds();
  
  // Turn to the left 45 degrees.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while((int32_t)turnAngle < turnAngle45)
  {
    proxSensors.read();
    if(proxSensors.countsFrontWithLeftLeds()>maxLeftValue){
      maxLeftValue = proxSensors.countsFrontWithLeftLeds();
    }
    turnSensorUpdate();
  }

  // Turn back to center using the gyro.
  motors.setSpeeds(calibrationSpeed, -calibrationSpeed);
  while((int32_t)turnAngle > 0)
  {
    proxSensors.read();
    if(proxSensors.countsFrontWithLeftLeds()>maxLeftValue){
      maxLeftValue = proxSensors.countsFrontWithLeftLeds();
    }
    turnSensorUpdate();
  }
  return maxLeftValue;
}

// This function rotates the robot right by 45 degrees and back to the center while scanning for objects.
// Returns the maximum value found by the right proximity sensor
int scanRight()
{
  turnSensorReset();
  proxSensors.read();
  int maxRightValue = proxSensors.countsFrontWithRightLeds();
  
   // Turn to the right 45 degrees.
  motors.setSpeeds(calibrationSpeed, -calibrationSpeed);
  while((int32_t)turnAngle > -turnAngle45)
  {
    proxSensors.read();
    if(proxSensors.countsFrontWithRightLeds()>maxRightValue){
      maxRightValue = proxSensors.countsFrontWithLeftLeds();
    }
    turnSensorUpdate();
  }

  // Turn back to center using the gyro.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while((int32_t)turnAngle < 0)
  {
    proxSensors.read();
    if(proxSensors.countsFrontWithRightLeds()>maxRightValue){
      maxRightValue = proxSensors.countsFrontWithLeftLeds();
    }
    turnSensorUpdate();
  }
  return maxRightValue;
}
/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
