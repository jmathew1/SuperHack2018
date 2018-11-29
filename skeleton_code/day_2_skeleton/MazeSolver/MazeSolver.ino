#include <Wire.h>
#include <Zumo32U4.h>
#include "GridMovement.h"
#include "TurnSensor.h"

Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
L3G gyro;

// Holds data read from the proximity sensors
int maxLeftValue;
int maxRightValue;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Task 0: Choose a driving speed for the robot, from 0 to 400. You may need to change this as you complete tasks
int forwardSpeed = /* TO DO */;

// Task 3: Choose a proximity sensors threshold for civilian detection 
int proxSensorThreshold = /* TO DO */;
/*--------------- CONSTANTS FOR OPERATION ---------------*/

void setup()
{
  // Wait for button to be pressed, then start calibration
  buttonA.waitForButton();
  gridMovementSetup();
}

void loop()
{
  // Update the speeds of the motors to stay on the centre of the line
  updateMotorSpeeds();  // TASK 1: FILL IN THIS FUNCTION

  // While driving, check for any dead ends or interesections
  if(deadEnd() || intersection())
  {
		// Drive straight forward to get to the center of the intersection, check for left, right and straight exits
 		bool foundLeft, foundStraight, foundRight;
  	driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);

    // If at T-intersection, we might be at the civilian room so check for civilians
    if(foundLeft&&foundRight&&!foundStraight){
  		if(civilianDetected())  // TASK 3a: FILL IN THIS FUNCTION
      {
        rescueCivilian()      // Task 3b: FILL IN THIS FUNCTION

        // After rescuing the civilian, stop the robot and wait for a button push to restart
        motors.setSpeeds(0,0);
        buttonA.waitForButton(); 
      }
    }
  		// Choose a direction to turn.
  		char dir = selectTurn(foundLeft, foundStraight, foundRight); // TASK 2: FILL IN THIS FUNCTION
  
  		// Make the turn.
  		turn(dir);
    }
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/
// TASK 1: FILL IN THIS FUNCTION
void updateMotorSpeeds()
{
    // 1. Get the position of the robot by reading from the line sensors, like in the examples.

    // 2. Create a new variable called 'error' to figure out how far we are away from the centre of the line, based on the previous reading. 
    // If you have forgotten what position the centre of the line is, you might like to refer back to Part 1.

    // 3. Create a new variable called speedDifference. The value of speedDifference will be 'error', multiplyed by a number between 0 and 1
    // This will determine how much your robot reacts to the error. You might like to play around with different values. 
  
    // 4. Calculate the values for the left and right motor speeds, using your new speedDifference variable. 
    // You will need to add or subtract speedDifference to the robot's normal forward speed (defined at the start of the program).
    // HINT: Whether you add or subtract depends the motor (left or right), and how you defined your 'error' variable. It is up to you to figure it out. 
    // Try drawing a diagram of the robot in two scenarios: left of the centre of the line, and right of the centre. 
    // Would you get a positive or negative reading for speedDifference? If you're left of the line, which motor should have a greater speed than the other, in order to stay in the center of the line?
    // This should help you figure out whether to add or subtract for each motor
  
    // 6. Since we don't want the robot to go over the normal forward speed, use the in-built arduino 'constrain(1, 2, 3)' function to constrain the new left and right speeds between 0 and the forward speed.
    // The function takes 3 arguments: the variable you want to constrain, the lower bound to constrain and finally the upper bound to constrain.
    // The constrain function returns your constrained variable, which will be the exact speeds we need for the robot
    
    // 7. Set the speed of the motor using your newly calculated left and right speeds
}

// TASK 2: FILL IN THIS FUNCTION
char selectTurn(bool foundLeft, bool foundStraight, bool foundRight)
{
  /* TO DO */
}

// TASK 3a - FILL IN THIS FUNCTION
// This function returns TRUE if the robot detects a civilian
bool civilianDetected()
{
  /* TO DO */
}

// TASK 3b - FILL IN THIS FUNCTION
// This function faces towards the civilian, then slowly drives forward, pushing it until it is out of the room.
void rescueCivilian()
{
  /* TO DO */
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
