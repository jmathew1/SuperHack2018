#include <Wire.h>
#include <ZumoShield.h>

Pushbutton buttonA(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray lineSensors;

void setup() 
{
  // Wait for the user to press button A.
  buttonA.waitForButton();
  
  // Initialize the reflectance sensors as you did before

  // Perform calibration
  calibrateSensors();

  // Begin serial monitor here

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
}

void calibrateSensors()
{

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    lineSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
}
