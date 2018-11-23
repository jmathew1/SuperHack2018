#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Motor speed when turning during line sensor calibration
const uint16_t calibrationSpeed = 200;

// This line sensor threshold is used to dsetect intersections
const uint16_t sensorThreshold = 200;

// Delay to get the robot to the center of the intersection
const uint16_t intersectionDelay = /* TO DO */;

// The normal operating speed of the robot.
const uint16_t forwardSpeed = /* TO DO */;

// Holds the values of the 5 line sensors.
uint16_t lineSensorValues[5];
/*--------------- CONSTANTS FOR OPERATION ---------------*/

void setup()
{
	calibrateSensors();			// Calibrate the sensors

	buttonA.waitForButton();	// Wait for the button to be pressed
}

void loop()
{
	updateMotorSpeeds();	// TASK 1: FILL IN THIS FUNCTION
	
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
		bool foundLeft, foundStraight, foundRight;
		
		driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);	// TASK 2b: FILL IN THIS FUNCTION
	}
	
	// TASK 2c: FILL IN THESE FUNCTIONS
	if (detectedObject())	
	{
		moveAroundObject();
	}

	if(aboveDarkSpot())
    {
      // check if there is an object, if there is, finish and alert, if not, continue searching through maze
    }
	
    // Choose a direction to turn.
    char dir = selectTurn(foundLeft, foundStraight, foundRight);

    // Make the turn.
    turn(dir);
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/

/* TASK 1: LINE FOLLOWING - FILL IN THIS FUNCTION */
void updateMotorSpeeds()
{
	// 1. Get the position of the line by reading from the line sensors. Use data type uint16_t for your variable.

    // 2. Create a new variable called 'error' to figure out how far we are away from the centre of the line, based on the previous reading. The centre of the line corresponds to position 2000. Use the data type int16_t for your variable.
	//HINT: you might like to use the serial monitor to find out what readings you are getting from the line sensor, to figure out what your error variable should be.

	// 3. Create a new variable called errorChange. This variable will store the difference between the current error, and the last error. For now, you can call the last error variable 'lastError', as we will be defining it later. 

    // 4. Scale your error by multiplying it by a number between 0 and 1. This will determine how much your robot reacts to the error. You might like to play around with different values. 
	// Now, Add your scaled error to the change in error variable you created in 3. You will also need to scale errorChange, by multiplying it by number between 1 and 10.
	// You can call this new error variable speedDifference, with type int16_t

    // 5. Calculate the left and right motor speeds INDIVIDUALLY, using your speedDifference variable. You will need to add or subtract this variable to the robot's max speed. Whether you add or subtract depends on which motor and is up to you to figure it out. The serial monitor can help here. Both variables should be of type int16_t

    // 6. Since we don't want the robot to go over the max speed limit, use the in-built arduino 'constrain()' function to constrain the new left and right speeds between 0 and the max speed. 
	// The function takes 3 arguments: the variable you want to constrain, the lower bound to constrain and finally the upper bound to constrain.
	// You may reassign these constrained speeds to the old left and right speeds, or create new ones.
  
	// 7. Set the speed of the motor using your newly calculated left and right speeds!
}

// Returns TRUE if the sensor is seeing a line where sensorIndex is the sensor you wish to test.
bool lineExists(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}

/* TASK 2a: DEAD END TEST - FILL IN THIS FUNCTION */
// Use the lineExists function to test the various line sensors to see if there is a dead end in front of the robot 
bool deadEnd()
{
	/* TO DO */
	
	return /* TO DO */
}

/* TASK 2a: INTERSECTION TEST - FILL IN THIS FUNCTION */
// Use the lineExists function to test the various line sensors to see if there is an intersection in front of the robot
bool intersection()
{	
	/* TO DO */
	
	return /* TO DO */
}

/* TASK 2b: DRIVE TO CENTER OF INTERSECTION - FILL IN THIS FUCTION */
// Drive straight forward to get to the center of the intersection, then check for possible exits
void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
	*foundLeft = 0;
	*foundStraight = 0;
	*foundRight = 0;

	motors.setSpeeds(straightSpeed, straightSpeed);
	
	// 1. Modify the value of intersectionDelay at the start of this program so the robot reaches the center of the intersection
	delay(intersectionDelay);	
	
    lineSensors.readLine(lineSensorValues);	// Read the values of the line sensors
	
	// 2. Using the lineExists function above, test to see if the first (leftmost) lineSensor detects a line
    if(/* TO DO */)
    {
      *foundLeft = 1;
    }
	
	// 3. Using the lineExists function, test to see if the last (rightmost) lineSensor detects a line
    if(/* TO DO */)
    {
      *foundRight = 1;
    }

  lineSensors.readLine(lineSensorValues);	// Read the values again, just to be sure

	// 4. Using the lineExists function, test to see if any of the middle 3 Sensors detects a line
	if(/* TO DO */)
	{
		*foundStraight = 1;
	}
}
/*------------------------------------ END FUNCTIONS TO FILL IN ------------------------------------*/








/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
void calibrateSensors()
{
	// Wait 1 second and then begin automatic sensor calibration
	// by rotating in place to sweep the sensors over the line
	delay(1000);
	for(uint16_t i = 0; i < 120; i++)
	{
		if (i > 30 && i <= 90)
		{
			motors.setSpeeds(-200, 200);
		}
		else
		{
			motors.setSpeeds(200, -200);
		}

		lineSensors.calibrate();
	}
	
	motors.setSpeeds(0, 0);
}

// This function decides which way to turn.
char selectTurn(bool foundLeft, bool foundStraight, bool foundRight)
{
  /* POSSIBLY REMOVE: REMOVE CODE THAT GIVES LEFT HAND RULE */
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if(foundLeft) { return 'L'; }
  else if(foundStraight) { return 'S'; }
  else if(foundRight) { return 'R'; }
  else { return 'B'; }
}

// Turns according to the parameter dir, which should be 'L'
// (left), 'R' (right), 'S' (straight), or 'B' (back).  We turn
// most of the way using the gyro, and then use one of the line
// sensors to finish the turn.  We use the inner line sensor that
// is closer to the target line in order to reduce overshoot.
void turn(char dir)
{
  if (dir == 'S')
  {
    // Don't do anything!
    return;
  }

  turnSensorReset();

  uint8_t sensorIndex;

  switch(dir)
  {
  case 'B':
    // Turn left 125 degrees using the gyro.
    motors.setSpeeds(-turnSpeed, turnSpeed);
    while((int32_t)turnAngle < turnAngle45 * 3)
    {
      turnSensorUpdate();
    }
    sensorIndex = 1;
    break;

  case 'L':
    // Turn left 45 degrees using the gyro.
    motors.setSpeeds(-turnSpeed, turnSpeed);
    while((int32_t)turnAngle < turnAngle45)
    {
      turnSensorUpdate();
    }
    sensorIndex = 1;
    break;

  case 'R':
    // Turn right 45 degrees using the gyro.
    motors.setSpeeds(turnSpeed, -turnSpeed);
    while((int32_t)turnAngle > -turnAngle45)
    {
      turnSensorUpdate();
    }
    sensorIndex = 3;
    break;

  default:
    // This should not happen.
    return;
  }

  // Turn the rest of the way using the line sensors.
  while(1)
  {
    readSensors();
    if (aboveLine(sensorIndex))
    {
      // We found the line again, so the turn is done.
      break;
    }
  }
}

/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
