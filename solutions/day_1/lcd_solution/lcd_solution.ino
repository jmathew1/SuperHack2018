#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

#define SPEED_LIMIT 100

int current_speed;

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
  /** TASK II-IV **/
  for (current_speed = 0; current_speed <= 400; current_speed++)
  {
    motors.setSpeeds(current_speed, current_speed);
    //delay(2);
    ledGreen(1);
    ledRed(0);
    // Clear the screen then print the speed
    lcd.clear();
    lcd.print(current_speed);
  }

  delay(2000); // Hold this speed for 2 seconds before decelerating backwards

  for (current_speed = -400; current_speed <= 0; current_speed++)
  {
    motors.setSpeeds(current_speed, current_speed);
    //delay(2);
    ledGreen(0);
    ledRed(1);
  }
}
