#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

void setup()
{
  proxSensors.initFrontSensor();
  int front_left = 0;
  int front_right = 0;
  int constant_speed = 200;
  int min_distance = 1000; 
}

void loop() 
{
  if (front_left < min_distance || front_right < min_distance)
  {
    motors.setSpeeds(0, 0);
  }
  else
  {
    motors.setSpeeds(constant_speed, constant_speed);
  }
  
  proxSensors.read();
  int front_left = proxSensors.countsFrontWithLeftLeds();
  int front_right = proxSensors.countsFrontWithRightLeds();
}
