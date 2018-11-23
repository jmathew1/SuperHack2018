#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

#define CONSTANT_SPEED  100;
#define CIRCLE_DELAY    2000;
#define SPIN_DELAY      500;

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
  /** TASK I **/
  motors.setSpeeds(CONSTANT_SPEED, 0);
  delay(CIRCLE_DELAY);  // Modify this delay so the robot makes a complete circle
  motors.setSpeeds(0, 0);
  delay(5000);

  /** TASK II **/
  motors.setSpeeds(CONSTANT_SPEED, -CONSTANT_SPEED);
  delay(SPIN_DELAY);
  motors.setSpeeds(-CONSTANT_SPEED, CONSTANT_SPEED);
  delay(SPIN_DELAY);
  delay(5000);
}
