#include <Wire.h>
#include <ZumoShield.h>

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

void setup()
{
  // Wait for the user to press the button
  button.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop() 
{
  /* TASK 1 */


  /* TASK 2-4 */
}
