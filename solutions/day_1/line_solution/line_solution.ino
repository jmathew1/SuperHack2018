#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;  

#define NUM_SENSORS 5
uint16_t lineSensorValues[NUM_SENSORS];

void setup() 
{
  lineSensors.initFiveSensors();
  
  // Wait for the user to press button A.
  buttonA.waitForButton();

  // Delay so that the robot does not move away while the user is
  // still touching it.
  delay(1000);
}

void loop() 
{
  static uint16_t lastSampleTime = 0;

  if ((uint16_t)(millis() - lastSampleTime) >= 100)
  {
    lastSampleTime = millis();

    // Read the line sensors.
    lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
  }


}
