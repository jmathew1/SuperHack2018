#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

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
  /* TASK 1
  lcd.print("hello");
  lcd.gotoXY(0, 1);
  lcd.print(5);
  lcd.clear()
  */

  /* TASK 2 */
  for (int i = 0; i <= 400; i++)
  {
    motors.setSpeeds(i, i);
    lcd.print(i);
  }
  
}
