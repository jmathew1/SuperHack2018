#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ButtonA buttonA;

// Define your array here

void setup() 
{
  // Wait for the user to press button A.
  buttonA.waitForButton();
  
  // Initialize the line sensors here
  
  // Perform calibration
  calibrateSensors();

  // Wait for the user to press button A again.
  buttonA.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);

  lcd.clear();
}

void loop() 
{ 
  /* TASK 1 */

  /* TASK 2 */
  
  /* TASK 3 */
}

void calibrateSensors()
{
  lcd.clear();

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
