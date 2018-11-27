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

Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
L3G gyro;

// Task 0: choose a driving speed for the robot, from 0 to 400. You may need to change this  as you compelte tasks
int straightSpeed = 200;

void setup()
{
  buttonA.waitForButton();

  gridMovementSetup();
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

		/* IR CODE HERE */

		// Choose a direction to turn.
		char dir = selectTurn(foundLeft, foundStraight, foundRight);

		// Make the turn.
		turn(dir);
    }
}

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
    int leftSpeed = straightSpeed + speedDifference;
    int rightSpeed = straightSpeed - speedDifference;

    // Constrain our motor speeds to be between 0 and straightSpeed.
    leftSpeed = constrain(leftSpeed, 0, straightSpeed);
    rightSpeed = constrain(rightSpeed, 0, straightSpeed);

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
