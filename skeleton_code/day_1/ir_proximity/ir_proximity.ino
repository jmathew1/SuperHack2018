/* IR Proximity Sensor to scan for an object */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;

/* Be sure to define some constants here (e.g. robot's max speed, etc.) */

void setup() 
{
  startMessage();
  buttonA.waitForButton();

  /* Initialise the front proximity sensors here */

  lcd.clear();
}

void loop() {
  /* TASK 1 */
  /* Get sensors to emit an IR pulse */

  /* Assign the left and right sensor readings to a variable */

  /* Print values onto LCD*/

  /* TASK 2 */
  /* TIP: Using functions will help make your code look clearner and easier to debug, rather than writing everything in loop() */
}

/*--------------------DO NOT EDIT CODE BELOW THIS LINE--------------------*/
void startMessage() {
  lcd.clear();
  lcd.print("Press A");
  lcd.gotoXY(0,1);
  lcd.print("to begin!");
}
