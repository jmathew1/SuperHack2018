#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

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
  // Clear the screen
  lcd.clear();

  /** TASK II-IV **/
  int speed;
  for (speed = 0; speed <= 400; speed++)
  {
    lcd.print(speed);
    ledGreen(1);
    ledRed(0);
    motors.setSpeeds(speed, speed);
    delay(2);
  }

  delay(2000); // Hold this speed for 2 seconds before decelerating backwards

  for (speed = -400; speed <= 0; speed++)
  {
    lcd.print(speed);
    ledGreen(0);
    ledRed(1);
    motors.setSpeeds(speed, speed);
    delay(2);
  }



  // Print a string
  lcd.print("hi");

  // Go to the next line
  lcd.gotoXY(0, 1);

  // Print a number
  lcd.print(1234);

  delay(1000);
}
