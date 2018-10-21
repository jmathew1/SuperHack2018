#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

#define SPEED         100;
#define MIN_DISTANCE  1000; 
#define TURN_TIME     100;

void setup()
{
  proxSensors.initFrontSensor();
  int front_left = 0;
  int front_right = 0;
}

void loop() 
{
  /* TASK I */
  if (front_left < MIN_DISTANCE || front_right < MIN_DISTANCE)
  {
    motors.setSpeeds(0, 0);
    /* TASK II */
    motors.setSpeeds(SPEED, -SPEED);  // Turn clockwise on the spot
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED/2, SPEED/2);   // Move forward slightly
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(-SPEED, SPEED);  // Face forward
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED/2, SPEED/2);   // Move forward slightly
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED, -SPEED);  // Turn anti-clockwise on the spot
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED/2, SPEED/2);   // Move forward slightly
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED, -SPEED);  // Face forward
    delay(TURN_TIME);
    motors.setSpeeds(0, 0); // Stop
    delay(50);
    motors.setSpeeds(SPEED, SPEED);  Continue
  }
  else
  {
    motors.setSpeeds(SPEED, SPEED);
  }
  
  proxSensors.read();
  int front_left = proxSensors.countsFrontWithLeftLeds();
  int front_right = proxSensors.countsFrontWithRightLeds();
}
