#include <Wire.h>
#include <ZumoShield.h>

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;
ZumoBuzzer buzzer;

void setup() 
{
  // Initialize here
  
  // Wait for the user to press the button
  button.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop() 
{ 
  /* TASK 1 */

}
