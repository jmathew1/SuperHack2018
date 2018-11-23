#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4LCD lcd;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Motor speed when turning during line sensor calibration
const uint16_t calibrationSpeed = 200;

// This line sensor threshold is used to detect intersections
const uint16_t sensorThreshold = 200;

// Delay to get the robot to the center of the intersection
const uint16_t intersectionDelay = /* TO DO */;

// The normal operating speed of the robot. NOTE 200 IS THE SPEED LIMIT
const uint16_t forwardSpeed = /* TO DO */;

// Holds the values of the 5 line sensors.
uint16_t lineSensorValues[5];
/*--------------- CONSTANTS FOR OPERATION ---------------*/

void setup()
{
	calibrateSensors();
	
	showReadings();

	buttonA.waitForButton();
}

void loop()
{
	updateMotorSpeeds();	// TASK 1: FILL IN THIS FUNCTION
	
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
		bool foundLeft, foundStraight, foundRight;
		
		driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight);	// TASK 2b: FILL IN THIS FUNCTION
	}
	
    if(aboveDarkSpot())
    {
      // Found end of the end of the maze
      break;
    }

    // Choose a direction to turn.
    char dir = selectTurn(foundLeft, foundStraight, foundRight);

    // Make the turn.
    turn(dir);
}

/* TASK 1: FILL IN THIS FUNCTION */
void updateMotorSpeeds()
{
	// 1. Get the position of the line by reading from the line sensors. Use data type uint16_t for your variable.

    // 2. Create a new variable called 'error' to figure out how far we are away from the centre of the line, based on the previous reading. The centre of the line corresponds to position 2000. Use the data type int16_t for your variable.
	//HINT: you might like to use the serial monitor to find out what readings you are getting from the line sensor, to figure out what your error variable should be.

	// 3. Create a new variable called errorChange. This variable will store the difference between the current error, and the last error. For now, you can call the last error variable 'lastError', as we will be defining it later. 

    // 4. Scale your error by multiplying it by a number between 0 and 1. This will determine how much your robot reacts to the error. You might like to play around with different values. 
	// Now, Add your scaled error to the change in error variale you created in 3. You will also need to scale errorChange, by multiplying it by number between 1 and 10.
	// You can call this new error variable speedDifference, with type int16_t

    // 5. Calculate the left and right motor speeds INDIVIDUALLY, using your speedDifference variable. You will need to add or subtract this variable to the robot's max speed. Whether you add or subtract depends on which motor and is up to you to figure it out. The serial monitor can help here. Both variables should be of type int16_t

    // 6. Since we don't want the robot to go over the max speed limit, use the in-built arduino 'constrain()' function to constrain the new left and right speeds between 0 and the max speed. 
	// The function takes 3 arguments: the variable you want to constrain, the lower bound to constrain and finally the upper bound to constrain.
	// You may reassign these constrained speeds to the old left and right speeds, or create new ones.
  
	// 7. Set the speed of the motor using your newly calculated left and right speeds!
}

bool deadEnd()
{
	return /* TO DO */
}

bool intersection()
{
    return /* TO DO */
}

// Drive straight forward to get to the center of the intersection, then check for possible exits
void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
	*foundLeft = 0;
	*foundStraight = 0;
	*foundRight = 0;

	motors.setSpeeds(straightSpeed, straightSpeed);
	delay(intersectionDelay);
    lineSensors.readLine(lineSensorValues);
	
    if(aboveLine(0))
    {
      *foundLeft = 1;
    }
    if(aboveLine(4))
    {
      *foundRight = 1;
    }

  lineSensors.readLine(lineSensorValues);

  // Check for a straight exit.
  if(aboveLine(1) || aboveLine(2) || aboveLine(3))
  {
    *foundStraight = 1;
  }
}

/* ---- Below are functions that you do not need to modify ---- */
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

// Displays a bar graph of sensor readings on the LCD until button A is pressed
void showReadings()
{
  lcd.clear();

  while(!buttonA.getSingleDebouncedPress())
  {
    lineSensors.readCalibrated(lineSensorValues);

    lcd.gotoXY(0, 0);
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
      uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
      printBar(barHeight);
    }
  }
}

// Returns TRUE if the sensor is seeing a line where sensorIndex is the sensor you wish to test
bool aboveLine(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThreshold;
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