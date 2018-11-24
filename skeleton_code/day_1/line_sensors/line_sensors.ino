#include <Wire.h>
#include <ZumoShield.h>

Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;

void setup() 
{
  // Initialize here

  // Begin serial monitor
  
  // Wait for the user to press button A.
  buttonA.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop() 
{ 
  /* TASK 1 */

  /* TASK 2 */

  /* TASK 3 */
}
