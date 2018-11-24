#include <Wire.h>
#include <ZumoShield.h>

Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;
ZumoBuzzer buzzer;

/*--------------- CONSTANTS FOR OPERATION ---------------*/
// Threshold to detect intersections
int sensorThreshold = 300;

// The normal operating speed of the robot.
int forwardSpeed = 400;

// Thickness of the line in inchese
int lineThickness = 0.70;
int inches_to_zunits = 17142.0;

// Holds the values of the 6 line sensors.
int lineSensorValues[6];

// For Task 1
int lastError = 0;

// For Task 2a: helps the robot pick a direction to turn
bool foundLeft, foundStraight, foundRight;
/*--------------- CONSTANTS FOR OPERATION ---------------*/

void setup()
{
  lineSensors.init();
  delay(500);
  
  buttonA.waitForButton();  // Wait for the button to be pressed
  
  calibrateSensors();     // Calibrate the sensors
}

void loop()
{
    updateMotorSpeeds();  // TASK 1: FILL IN THIS FUNCTION
  
    if(deadEnd() || intersection()) // TASK 2a: FILL IN THESE FUNCTIONS
    {
      // Found an intersection
      driveToIntersectionCenter(&foundLeft, &foundStraight, &foundRight); // TASK 2b: FILL IN THIS FUNCTION

      // Move a little bit more forward to check to see if there is a dead end
      delay(overshoot(lineThickness)*5);
      
      if(lineExists(0) && lineExists(1) && lineExists(2) && lineExists(3) && lineExists(4) && lineExists(5))
      {
        // Found end of the end of the maze
        motors.setSpeeds(0, 0);
        delay(60000);
      }
      else
      {
          char dir = selectTurn(foundLeft, foundStraight, foundRight);
    
          // Make the turn.
          turn(dir);
      }
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
/*------------------------------------ END FUNCTIONS TO FILL IN ------------------------------------*/








/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
void calibrateSensors()
{
  unsigned short count = 0;
  unsigned short last_status = 0;
  int turn_direction = 1;
  
  // Calibrate the Zumo by sweeping it from left to right
  for(int i = 0; i < 4; i ++)
  {
    // Zumo will turn clockwise if turn_direction = 1.
    // If turn_direction = -1 Zumo will turn counter-clockwise.
    turn_direction *= -1;

    // Turn direction.
    motors.setSpeeds(turn_direction * forwardSpeed, -1*turn_direction * forwardSpeed);

    // This while loop monitors line position
    // until the turn is complete.
    while(count < 2)
    {
      lineSensors.calibrate();
      lineSensors.readLine(lineSensorValues);
      if(turn_direction < 0)
      {
        // If the right  most sensor changes from (over white space -> over
        // line or over line -> over white space) add 1 to count.
        count += lineExists(5) ^ last_status;
        last_status = lineExists(5);
      }
      else
      {
        // If the left most sensor changes from (over white space -> over
        // line or over line -> over white space) add 1 to count.
        count += lineExists(0) ^ last_status;
        last_status = lineExists(0);
      }
    }

    count = 0;
    last_status = 0;
  }
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
     
      motors.setSpeeds(forwardSpeed, -forwardSpeed);

      // This while loop monitors line position
      // until the turn is complete.
      while(count < 2)
      {
        lineSensors.readLine(lineSensorValues);

        // Increment count whenever the state of the sensor changes
    // (white->black and black->white) since the sensor should
    // pass over 1 line while the robot is turning, the final
    // count should be 2
        count += lineExists(lineSensorValues[1]) ^ last_status;
        last_status = lineExists(lineSensorValues[1]);
      }

    break;

    case 'R':
      // Turn right.
      motors.setSpeeds(-forwardSpeed, forwardSpeed);

      // This while loop monitors line position
      // until the turn is complete.
      while(count < 2)
      {
        lineSensors.readLine(lineSensorValues);
        count += lineExists(lineSensorValues[4]) ^ last_status;
        last_status = lineExists(lineSensorValues[4]);
      }

    break;

    case 'S':
    // Don't do anything!
    break;
  }
}

int overshoot(int line_thickness)
{
  return (inches_to_zunits*(line_thickness))/forwardSpeed;
}
/*------------------------------------ GIVEN FUNCTIONS - DO NOT CHANGE ------------------------------------*/
