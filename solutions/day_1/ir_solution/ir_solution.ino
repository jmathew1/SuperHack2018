#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

#define SPEED         100;
#define MIN_DISTANCE  1000; 
#define TURN_TIME     100;

int front_left = 0;
int front_right = 0;

void setup()
{
  proxSensors.initFrontSensor();
}

void loop() 
{
  proxSensors.read();
  front_left = proxSensors.countsFrontWithLeftLeds();
  front_right = proxSensors.countsFrontWithRightLeds();
  
  /* TASK I */
  if (front_left < MIN_DISTANCE || front_right < MIN_DISTANCE)
  {
    motors.setSpeeds(SPEED, SPEED);
  {
  else
  {
    /* TASK I */
    //motors.setSpeeds(0, 0);
    /* TASK II */
    move_around_object();
  }

}

void move_around_object()
{
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
    motors.setSpeeds(SPEED, SPEED);  // Continue moving
}
}
