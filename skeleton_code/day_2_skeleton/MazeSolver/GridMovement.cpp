/* GridMovement.cpp and GridMovement.h provide functions for
moving on a line grid.  For more information, see
GridMovement.h. */

#include "TurnSensor.h"
#include "GridMovement.h"

uint16_t lineSensorValues[numSensors];

void printBar(uint8_t height)
{
  if (height > 8) { height = 8; }
  const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, 255};
  lcd.print(barChars[height]);
}

// Returns true if the sensor is seeing a lot of darkness.
// Make sure to call readSensors() before calling this.
bool aboveLineDark(uint8_t sensorIndex)
{
  return lineSensorValues[sensorIndex] > sensorThresholdDark;
}

// Checks to see if we are over a dark spot, like the ones used
// to mark the end of a maze.  If all the middle sensors are over
// dark black, that means we have found the spot.
// Make sure to call readSensors() before calling this.
bool aboveDarkSpot()
{
  return aboveLineDark(1) && aboveLineDark(2) && aboveLineDark(3);
}

bool deadEnd()
{
  return !aboveLine(0) && !aboveLine(1) && !aboveLine(2) && !aboveLine(3) && !aboveLine(4);
}

bool intersection()
{
  return aboveLine(0) || aboveLine(4);
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

// This should be called after followSegment to drive to the
// center of an intersection.
void driveToIntersectionCenter()
{
  // Drive to the center of the intersection.
  motors.setSpeeds(straightSpeed, straightSpeed);
  delay(intersectionDelay);
}

void driveToIntersectionCenter(bool * foundLeft, bool * foundStraight, bool * foundRight)
{
  *foundLeft = 0;
  *foundStraight = 0;
  *foundRight = 0;

  // Drive stright forward to get to the center of the
  // intersection, while simultaneously checking for left and
  // right exits.
  
  // readSensors() takes approximately 2 ms to run, so we use
  // it for our loop timing.  A more robust approach would be
  // to use millis() for timing.
  int count = 0;
  motors.setSpeeds(straightSpeed, straightSpeed);
  while(aboveLine(0)||aboveLine(4)) // Keep moving foward till the line sensors no longer detect a line to either the left or right 
  {
    readSensors();
    count +=1;
    if(aboveLine(0))
    {
      *foundLeft = 1;
    }
    if(aboveLine(4))
    {
      *foundRight = 1;
    }
  }

  // Move forward an additional 2 line thicknesses so the robot is centered at the intersection
  count = count*2;
  while(count){
    count -=1;
    delay(2);
  }
  readSensors();

  // Check for a straight exit.
  if(aboveLine(1) || aboveLine(2) || aboveLine(3))
  {
    *foundStraight = 1;
  }
}

/* ======================= CALIBRATION =======================*/
// Calibrates the line sensors by turning left and right, then
// displays a bar graph of calibrated sensor readings on the LCD.
// Returns after the user presses A.
static void lineSensorSetup()
{
  lcd.clear();
  lcd.print(F("Line cal"));

  // Delay so the robot does not move while the user is still
  // touching the button.
  delay(1000);

  // We use the gyro to turn so that we don't turn more than
  // necessary, and so that if there are issues with the gyro
  // then you will know before actually starting the robot.

  turnSensorReset();

  // Turn to the left 90 degrees.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while((int32_t)turnAngle < turnAngle45 * 2)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
  }

  // Turn to the right 90 degrees.
  motors.setSpeeds(calibrationSpeed, -calibrationSpeed);
  while((int32_t)turnAngle > -turnAngle45 * 2)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
  }

  // Turn back to center using the gyro.
  motors.setSpeeds(-calibrationSpeed, calibrationSpeed);
  while((int32_t)turnAngle < 0)
  {
    lineSensors.calibrate();
    turnSensorUpdate();
  }

  // Stop the motors.
  motors.setSpeeds(0, 0);

  // Show the line sensor readings on the LCD until button A is
  // pressed.
  lcd.clear();
  while(!buttonA.getSingleDebouncedPress())
  {
    readSensors();

    lcd.gotoXY(0, 0);
    for (uint8_t i = 0; i < numSensors; i++)
    {
      uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
      printBar(barHeight);
    }
  }

  lcd.clear();
}

void gridMovementSetup()
{
  // Configure the pins used for the line sensors.
  lineSensors.initFiveSensors();

  // Set up custom characters on the LCD so we can show a bar
  // graph of the sensor readings after calibration.
 // loadCustomCharacters();

  // Calibrate the gyro and show readings from it until the user
  // presses button A.
  turnSensorSetup();

  // Calibrate the sensors by turning left and right, and show
  // readings from it until the user presses A again.
  lineSensorSetup();
}

// Sets up special characters in the LCD so that we can display
// bar graphs.
static void loadCustomCharacters()
{
  static const char levels[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
  };
  lcd.loadCustomCharacter(levels + 0, 0);  // 1 bar
  lcd.loadCustomCharacter(levels + 1, 1);  // 2 bars
  lcd.loadCustomCharacter(levels + 2, 2);  // 3 bars
  lcd.loadCustomCharacter(levels + 3, 3);  // 4 bars
  lcd.loadCustomCharacter(levels + 4, 4);  // 5 bars
  lcd.loadCustomCharacter(levels + 5, 5);  // 6 bars
  lcd.loadCustomCharacter(levels + 6, 6);  // 7 bars
}
/* ======================= CALIBRATION =======================*/
