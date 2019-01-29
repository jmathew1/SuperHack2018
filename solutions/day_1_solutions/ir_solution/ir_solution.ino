/* IR Proximity Sensor to scan for an object */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;

/* USEFUL METHODS:
 *  <sensor>.initFrontSensor(); - Configures this object to use just the front proximity sensors (front left and front right) 
 *  <sensor>.initThreeSensor(); - Configures this object to use all three proximity sensors (front, left and right)
 *  <sensor>.read(); - Makes the sensors emit IR pulses and get readings from the sensors
 *  <sensor>.countsFrontWithLeftLeds(); 
 *  <sensor>.countsFrontWithRightLeds(); 
 */

const int sensorThreshold = 6;
const int speed = 250;

void setup() {
  startMessage();
  buttonA.waitForButton();

  // Intialising proximity sensors
  proxSensors.initFrontSensor();


  lcd.clear();
}

void loop() {
  // Read the front proximity sensors
  proxSensors.read();

  // Assigning front left and right proximity sensors to a variable - what do the values represent? - the brighter the LED is reflected back, the closer the object, hence, the higher value for a closer object
  int leftSensor = proxSensors.countsFrontWithLeftLeds();     // Returns the number of brightness levels for the LEFT LEDs that activated the front proximity sensor
  int rightSensor = proxSensors.countsFrontWithRightLeds();   // Returns the number of brightness levels for the RIGHT LEDs that activated the front proximity sensor.

  // Print left and right sensor values onto the LCD
  printLCD(leftSensor, rightSensor);

  driveToObstacle(leftSensor, rightSensor);  

}

void driveToObstacle(int leftSensor, int rightSensor) {
  motors.setSpeeds(speed, speed);
  if (leftSensor >= sensorThreshold || rightSensor >= sensorThreshold) {
    motors.setSpeeds(0, 0);
  }
}

void printLCD(int leftSensor, int rightSensor) {
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print("L: ");
  lcd.print(leftSensor);
  lcd.gotoXY(0,1);
  lcd.print("R: ");
  lcd.print(rightSensor);  
}

void startMessage() {
  lcd.clear();
  lcd.print("Press A");
  lcd.gotoXY(0,1);
  lcd.print("to begin!");
}

