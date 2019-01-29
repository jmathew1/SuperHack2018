#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

int constant_speed = 200;
int circle_delay = 2000;  /* may not be correct, needs to be tested */
int spin_delay = 100;     /* may not be correct, needs to be tested */

void setup()
{
  // Wait for the user to press button A.
  buttonA.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop()
{
  /** TASK I **/
  motors.setSpeeds(constant_speed, 0);
  delay(circle_delay);  // Modify this delay so the robot makes a complete circle
  motors.setSpeeds(0, 0);
  delay(5000);

  /** TASK II **/
  motors.setSpeeds(constant_speed, -constant_speed);
  delay(spin_delay);
  motors.setSpeeds(-constant_speed, constant_speed);
  delay(spin_delay);
  delay(5000);
}
