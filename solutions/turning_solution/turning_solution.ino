#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

void setup()
{
  // Uncomment if necessary to correct motor directions:
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);

  // Wait for the user to press button A.
  buttonA.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop()
{
  int constant_speed = 100;
  /** TASK I **/
  int circle_delay = 2000;
  motors.setSpeeds(constant_speed, 0);
  delay(circle_delay);  // Modify this delay so the robot makes a complete circle
  motors.setSpeeds(0, 0);
  delay(50);

  /** TASK II **/
  int spin_delay = 500;
  motors.setSpeeds(constant_speed, -constant_speed);
  delay(spin_delay);
  motors.setSpeeds(-constant_speed, constant_speed);
  delay(spin_delay);
  delay(50);
}
