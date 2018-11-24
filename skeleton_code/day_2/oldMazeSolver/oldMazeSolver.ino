#include <Wire.h>
#include <ZumoShield.h>


Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;


/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Threshold to detect intersections
int sensorThreshold = 300;

// Thickness of the line in inchese
int lineThickness = 0.70;
int inches_to_zunits = 17142.0;

// Holds the values of the 6 line sensors.
int lineSensorValues[6];

// Control variable for Task 1
int lastError = 0;

// For Task 2a: helps the robot pick a direction to turn
bool foundLeft, foundStraight, foundRight;

// The normal operating speed of the robot, from 0 to 400
int forwardSpeed = /* TO DO */
/*--------------- CONSTANTS FOR OPERATION ---------------*/


void setup()
{
	lineSensors.init();			// Initialize the sensors
	delay(500);
	
	buttonA.waitForButton();	// Wait for the button to be pressed
	
	calibrateSensors();			// Calibrate the sensors, then start driving
}

void loop()
{
	// First, update the motor speeds so it stays on the centre 
	updateMotorSpeeds();	// TASK 1: FILL IN THIS FUNCTION
	
	// Check as we are driving to see if there are any dead ends or places to turn
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
		// Found one, move forward a little to get into the center of the intersection/dead end and check again
		driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);	
		
		// Choose a direction to turn, based on which exits are available
		char dir = selectTurn(foundLeft, foundStraight, foundRight);

		// Make the turn.
		turn(dir);
	}
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/

/* TASK 1: LINE FOLLOWING - FILL IN THIS FUNCTION */
void updateMotorSpeeds()
{
	// 1. Get the position of the line by reading from the line sensors, like in the examples.

    // 2. Create a new variable called 'error' to figure out how far we are away from the centre of the line, based on the previous reading. If you have forgotten what position the centre of the line is, you might like to refer back to Part 1.

	// 3. Create a new variable called errorChange. This variable will store the difference between the current error and the last error. For now, you can call the last error variable 'lastError', as we will be defining it later. 

    // 4a. Create a new variable called speedDifference. The value of speedDifference will be 'error', multiplyed by a number between 0 and 1, added to the change in error variable 'errorChange', but scaled by a number between 1 and 10.
	// This will determine how much your robot reacts to the error. You might like to play around with different values. 
	
    // 4b. Set lastError = error

    // 5. Calculate the values for the left and right motor speeds, using your new speedDifference variable. 
	// You will need to add or subtract speedDifference to the robot's normal forward speed (defined at the start of the program).
	// HINT: Whether you add or subtract depends the motor, and how you defined your 'error' variable. It is up to you to figure it out. 
	// Try drawing a diagram of the robot in two scenarios: left of the centre of the line, and right of the centre. 
	// Would you get a positive or negative reading for speedDifference? If you're left of the line, which motor do you want to move forwards and which one do you want to move backwards?
	// This should help you figure out whether to add or subtract for each motor

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
	
	//return /* TO DO */
}

/* TASK 2a: INTERSECTION TEST - FILL IN THIS FUNCTION */
// Use the lineExists function to test the various line sensors to see if there is an intersection in front of the robot. You will need to figure out which individual line sensors may be useful to check.
// This function should return TRUE if there is an intersection.
bool intersection()
{	
	/* TO DO */
	
	//return /* TO DO */
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

void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
  *foundLeft = 0;
  *foundStraight = 0;
  *foundRight = 0;

  // Check for left and right exits again
  lineSensors.readLine(lineSensorValues);
  if(lineExists(0))
    foundLeft = 1;
  if(lineExists(5))
    foundRight = 1;


   // Drive straight a bit more, until we are
        // approximately in the middle of intersection.
        // This should help us better detect if we
        // have left or right segments.
        motors.setSpeeds(forwardSpeed, forwardSpeed);
        delay(overshoot(lineThickness)/2);

        lineSensors.readLine(lineSensorValues);

        // Check for left and right exits.
        if(lineExists(0))
           foundLeft = 1;
         if(lineExists(5))
             foundRight = 1;


        // After driving a little further, we
        // should have passed the intersection
        // and can check to see if we've hit the
        // finish line or if there is a straight segment
        // ahead.
        delay(overshoot(lineThickness)/2);
        
        lineSensors.readLine(lineSensorValues);

        // Check for left and right exits.
        if(lineExists(0))
           foundLeft = 1;
         if(lineExists(5))
             foundRight = 1;


        if(lineExists(1) || lineExists(2) || lineExists(3) || lineExists(4))
            foundStraight = 1;
}

int overshoot(int line_thickness)
{
  return (inches_to_zunits*(line_thickness))/forwardSpeed;
}
/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/