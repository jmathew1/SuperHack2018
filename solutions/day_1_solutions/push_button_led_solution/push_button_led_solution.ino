#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

void setup() 
{
  
}

void loop() 
{
  /* TASK 1 
  buttonA.waitForButton()
  ledRed(1);
  delay(1000);
  ledRed(0)
  delay(1000);
  ledRed(1);
  delay(1000);
  ledRed(0)
  delay(1000);
  */
  
  /* TASK 2 */
  if (buttonA.isPressed())
  {
    ledRed(1);
    ledGreen(0);
    ledYellow(0);
  }

  if (buttonB.isPressed())
  {
    ledRed(0);
    ledGreen(1);
    ledYellow(0);
  }

  if (buttonC.isPressed())
  {
    ledRed(0);
    ledGreen(0);
    ledYellow(1);
  }
}
