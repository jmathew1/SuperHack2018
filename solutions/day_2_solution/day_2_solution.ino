#include <Wire.h>
#include <ZumoShield.h>

Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Motor speed when turning during line sensor calibration
const uint16_t calibrationSpeed = 150;

// This line sensor threshold is used to dsetect intersections
const uint16_t sensorThreshold = 300;

// Delay to get the robot to the center of the intersection
const uint16_t intersectionDelay = 50;

// The normal operating speed of the robot.
const uint16_t forwardSpeed = 300;

// Holds the values of the 5 line sensors.
uint16_t lineSensorValues[6];
/*--------------- CONSTANTS FOR OPERATION ---------------*/

bool foundLeft, foundStraight, foundRight;
int turndelay = 120;
int sensorThresholdDark = 600;
int lastError = 0;

void setup()
{
  lineSensors.init();
  
  buttonA.waitForButton();  // Wait for the button to be pressed
  
  calibrateSensors();     // Calibrate the sensors

  // Wait for the user button to be pressed and released
  buttonA.waitForButton();

}

void loop()
{
    updateMotorSpeeds();  // TASK 1: FILL IN THIS FUNCTION
  
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
      driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight); // TASK 2b: FILL IN THIS FUNCTION
      
      // Choose a direction to turn.
      if(aboveDarkSpot())
      {
        // Found end of the end of the maze
        motors.setSpeeds(0, 0);
        delay(60000);
       }
      
      char dir = selectTurn(foundLeft, foundStraight, foundRight);

      // Make the turn.
      turn(dir);
    }
}

/*------------------------------------ START FUNCTIONS TO FILL IN ------------------------------------*/

/* TASK 1: LINE FOLLOWING - FILL IN THIS FUNCTION */
void updateMotorSpeeds()
{
  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = lineSensors.readLine(lineSensorValues);

  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = forwardSpeed + speedDifference;
  int m2Speed = forwardSpeed - speedDifference;

  // Here we constrain our motor speeds to be between 0 and forwardSpeed.
  // Generally speaking, one motor will always be turning at forwardSpeed
  // and the other will be at forwardSpeed-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you might want to
  // allow the motor speed to go negative so that it can spin in reverse.
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > forwardSpeed)
    m1Speed = forwardSpeed;
  if (m2Speed > forwardSpeed)
    m2Speed = forwardSpeed;

  motors.setSpeeds(m1Speed, m2Speed);
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
  return (!lineExists(0) && !lineExists(1) && !lineExists(2) && !lineExists(3) && !lineExists(4));
}

/* TASK 2a: INTERSECTION TEST - FILL IN THIS FUNCTION */
// Use the lineExists function to test the various line sensors to see if there is an intersection in front of the robot
bool intersection()
{ 
  return (lineExists(0) || lineExists(5));
}

/* TASK 2b: DRIVE TO CENTER OF INTERSECTION - FILL IN THIS FUCTION */
// Drive straight forward to get to the center of the intersection, then check for possible exits
void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
  *foundLeft = 0;
  *foundStraight = 0;
  *foundRight = 0;

  motors.setSpeeds(forwardSpeed, forwardSpeed);
  
  // 1. Modify the value of intersectionDelay at the start of this program so the robot reaches the center of the intersection
  delay(intersectionDelay); 
  
    lineSensors.readLine(lineSensorValues); // Read the values of the line sensors
  
  // 2. Using the lineExists function above, test to see if the first (leftmost) lineSensor detects a line
    if(lineExists(0))
    {
      *foundLeft = 1;
    }
  
  // 3. Using the lineExists function, test to see if the last (rightmost) lineSensor detects a line
    if(lineExists(4))
    {
      *foundRight = 1;
    }

  lineSensors.readLine(lineSensorValues); // Read the values again, just to be sure

  // 4. Using the lineExists function, test to see if any of the middle 3 Sensors detects a line
  if(lineExists(1) || lineExists(2) || lineExists(3))
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
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    lineSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

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
// Turns according to the parameter dir, which should be
// 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
void turn(char dir)
{

  // count and last_status help
  // keep track of how much further
  // the Zumo needs to turn.
  unsigned short count = 0;
  unsigned short last_status = 0;
  unsigned int sensors[6];

  // dir tests for which direction to turn
  switch(dir)
  {

  // Since we're using the sensors to coordinate turns instead of timing them,
  // we can treat a left turn the same as a direction reversal: they differ only
  // in whether the zumo will turn 90 degrees or 180 degrees before seeing the
  // line under the sensor. If 'B' is passed to the turn function when there is a
  // left turn available, then the Zumo will turn onto the left segment.
    case 'L':
  case 'B':
      // Turn left.
      motors.setSpeeds(-forwardSpeed, forwardSpeed);

      // This while loop monitors line position
      // until the turn is complete.
      while(count < 2)
      {
        lineSensors.readLine(sensors);

        // Increment count whenever the state of the sensor changes
    // (white->black and black->white) since the sensor should
    // pass over 1 line while the robot is turning, the final
    // count should be 2
        count += lineExists(sensors[1]) ^ last_status;
        last_status = lineExists(sensors[1]);
      }

    break;

    case 'R':
      // Turn right.
      motors.setSpeeds(forwardSpeed, -forwardSpeed);

      // This while loop monitors line position
      // until the turn is complete.
      while(count < 2)
      {
        lineSensors.readLine(sensors);
        count += lineExists(sensors[4]) ^ last_status;
        last_status = lineExists(sensors[4]);
      }

    break;

    case 'S':
    // Don't do anything!
    break;
  }
}


bool aboveDarkSpot()
{
  return aboveLineDark(1) && aboveLineDark(2) && aboveLineDark(3);
}

bool aboveLineDark(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThresholdDark;
}

/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
