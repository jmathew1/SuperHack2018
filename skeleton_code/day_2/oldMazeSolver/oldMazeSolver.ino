#include <Wire.h>
#include <ZumoShield.h>

Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Motor speed when turning during line sensor calibration
int calibrationSpeed = 150;

// This line sensor threshold is used to dsetect intersections
int sensorThreshold = 300;

// Delay when robot turns at an intersection
int turndelay = 120;

// Holds the values of the 6 line sensors.
int lineSensorValues[6];

// Variables to detect intersection paths
bool foundLeft, foundStraight, foundRight;

// Used to detect dark zones
int sensorThresholdDark = 600;

// For Task 1
int lastError = 0;

// Delay to get the robot to the center of the intersection (millisec)
int intersectionDelay = /* TO DO - SEE TASK 2b */;

// The normal operating speed of the robot (0 to 400)
int forwardSpeed = /* TO DO */;
/*--------------- CONSTANTS FOR OPERATION ---------------*/


void setup()
{
	lineSensors.init();			// Initialize the sensors
	
	buttonA.waitForButton();	// Wait for the button to be pressed
	
	calibrateSensors();			// Calibrate the sensors
}

void loop()
{
	updateMotorSpeeds();	// TASK 1: FILL IN THIS FUNCTION
	
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
		driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);	// TASK 2b: FILL IN THIS FUNCTION
		
		if(aboveDarkSpot())
		{
		  // Found end of the end of the maze
		  motors.setSpeeds(0, 0);
		  delay(10000);
		}
		
		
		// Choose a direction to turn.
		char dir = selectTurn(foundLeft, foundStraight, foundRight);

		// Make the turn.
		turn(dir);
	}
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/

/* TASK 1: LINE FOLLOWING - FILL IN THIS FUNCTION */
void updateMotorSpeeds()
{
	// 1. Get the position of the line by reading from the line sensors.

    // 2. Create a new variable called 'error' to figure out how far we are away from the centre of the line, based on the previous reading. Note that the centre of the line corresponds to position 2000. 

	// 3. Create a new variable called errorChange. This variable will store the difference between the current error and the last error. For now, you can call the last error variable 'lastError', as we will be defining it later. 

    // 4. Create a new variable called speedDifference. The value of speedDifference will be your error variable, multiplyed by a number between 0 and 1, added to the change in error variable you created in 3., but scaled by a number between 1 and 10.
	// This will determine how much your robot reacts to the error. You might like to play around with different values. 

    // 5. Calculate the left and right motor speeds INDIVIDUALLY, using your speedDifference variable. You will need to add or subtract this variable to the robot's normal forward speed (defined at the start of the program). Whether you add or subtract depends on which motor and is up to you to figure it out. The serial monitor can help here. 

    // 6. Since we don't want the robot to go over the normal forward speed, use the in-built arduino 'constrain(1, 2, 3)' function to constrain the new left and right speeds between 0 and the forward speed.
	// The function takes 3 arguments: the variable you want to constrain, the lower bound to constrain and finally the upper bound to constrain.
	// The constrain function returns your constrained variable, which will be the exact speeds we need for the robot
  
	// 7. Set the speed of the motor using your newly calculated left and right speeds
}

// Returns TRUE if a particular line sensor is seeing a line, where the input sensorIndex is the sensor number you wish to test. You can remind yourself how many sensors there are by looking underneath the robot
bool lineExists(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
}

/* TASK 2a: DEAD END TEST - FILL IN THIS FUNCTION */
// Use the lineExists function above to test the various line sensors to see if there is a dead end in front of the robot. You will need to figure out which individual line sensors may be useful to check.
// This function should return TRUE if there is a dead end.
bool deadEnd()
{
	/* TO DO */
	
	return /* TO DO */
}

/* TASK 2a: INTERSECTION TEST - FILL IN THIS FUNCTION */
// Use the lineExists function to test the various line sensors to see if there is an intersection in front of the robot. You will need to figure out which individual line sensors may be useful to check.
// This function should return TRUE if there is an intersection.
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
	
	// 1. Modify the value of intersectionDelay at the start of this program so the robot reaches the center of the intersection when it sees one
	// HINT: you may like to add a delay() (or play a sound!) BEFORE this function is called in the main loop to make it more clear how far the robot moves when it detects an intersection
	delay(intersectionDelay);	
	
    lineSensors.readLine(lineSensorValues);	// Read the values of the line sensors
	
	// 2. Using the lineExists function above, test to see if the first (leftmost) lineSensor detects a line
	// HINT: This part is very similar to 2a
    if(/* TO DO */)
    {
      *foundLeft = 1;
    }
	
	// 3. Using the lineExists function, test to see if the last (rightmost) lineSensor detects a line
	// HINT: This part is very similar to 2a
    if(/* TO DO */)
    {
      *foundRight = 1;
    }

	lineSensors.readLine(lineSensorValues);	// Read the values again

	// 4. Using the lineExists function, test to see if any of the middle 3 Sensors detects a line
	// HINT: this part is very similar to 2a
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
