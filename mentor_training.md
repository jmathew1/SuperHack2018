# Mentor Training Day
This document contains the tasks required to complete on the mentor training day. As a reminder, **you are to solve these challenges WITHOUT referring to any examples in the Arduino IDE, or online.** You are encouraged to read through the zumo library and read some of the functions for each of the zumo classes (motors, buzzers, line sensors) to help you solve them.

If you haven't already, **download the Arduino IDE** at https://www.arduino.cc/en/Main/Software

If you haven't already, **download the zumo library** by opening the Arduino IDE and going to Sketch > Include Library > Manage Libraries then searching for **'ZumoShield'** and downloading it. 

The **zumo library reference** can be found at https://pololu.github.io/zumo-shield-arduino-library/ for your convenience and reading. You might want to keep this open while you complete the challenges.


## Part 1: Basic Programs
For each part, download and write your code in the the provided skeleton code in this repository under skeleton_code > day_1.

### Push Button and LED
1. Wait for the button to be pressed, then blink the LED repetedly. You may like to use the waitForButton() method in your Pushbutton object.
Note for this part you will need to use the pinMode and digitalWrite functions for the LED to turn it off and on, with the LED pin being 13. 

### Motor Control
1. Write a program to move the robot forwards at a constant velocity. Experiment with different values, with 400 being the maximum speed.
2. Modify the program to gradually accelerate the robot up to the maximum speed and maintain it for a few seconds. 
3. After that, decelerate the robot backwards from this max speed until it is stationary.
4. Turn the led on while the robot is accelerating, and turn it off when it is decelerating. 

### Turning
1. Move the robot forwards in a perfect circle so it loops back to its starting point (any size circle will do)
2. Once it reaches back, spin the robot slowly on the spot in the clockwise direction, then counterclockwise after it completes a full revolution

### Line Sensors 1
By looking underneath the robot, you will be able to see that there are 6 sensors at the front. These are used to detect black lines and are an important part of the main design challenge.
For this part, you will need to set up a few things before you can start. First, set up an integer global variable array of length 6. This array will store the values of the individual line sensors, after reading them.
In your arduino setup, you will need to initialize the line sensors using the init() method in your line sensor object (no arguments). 
You can find more details about the line sensors under ZumoReflectanceSensorArray in the main library above. 

1. Using the serial monitor, test to see what values you get on the line sensors when the robot encounters a piece of black tape. You will need to use the read() method in your line sensor object. It takes your array of line sensor values as an argument, and updates all of their individual values at once, each time it is called. For this part you don't need to drive the robot forward, just place it next to some tape and move it forward slowly until the line sensors are over the tape. Notice what happens to your serial monitor readings when this occurs. 
You may ignore the warnings when compiling and you might like to add a delay between readings.
2. Drive the robot forwards and make it stop when it sees a piece of black tape either side of it. You can comment out the above code.
3. Drive the robot forwards and make it stop when it sees a black piece of tape in front of it

### Line Sensors 2
Normally, the line sensors are calibrated properly before being used, rather than just getting the raw values as we did above. If you open the ZumoReflectanceSensorArray Class Reference on the zumo library reference website and scroll down to Detailed Description > Calibration, you will see that it is better to expose your robot to a 'calibration phase' before using the line sensors. Instead of holding the robot and moving it across the tape, we will do it by sweeping the motors and calibrating.
1. In the line sensors 2 skeleton code, you will find the provided calibration code. Have a read through it, then use the serial monitor to see the values you get on the line sensors, exactly as you did in the previous part. You will still need to initialize the sensors and create the global variable to hold your sensor values when reading them. Since the robot is calibrated, we can use the readLine() method instead, rather than just read(). It takes the same array as an argument. To test, place your zumo in the middle of some black tape while it performs calibration.
2. readLine() is important for line following applications, as it returns an integer that tells you where it thinks the line is. For this part, print in the serial monitor the output of readLine(). After the robot performs calibration, place the robot in the centre of a strip of black tape, then open the serial monitor and see what happens as you slowly slide the robot from left to right. This shows how readLine() can be useful in figuring out our position relative to the centre of the black line.

### Buzzer
1. Modify the above code to play a sound when it reaches a line. For this part it would help to read the ZumoBuzzer class description in the library
2. Read through and run ZumoBuzzerExample under File > Examples > ZumoShield

## Part 2: Design Challenge
For this part, download and write your code in the the provided skeleton code in this repository under skeleton_code > day_2 > oldMazeSolver.

Task 0: Decide on a normal operating speed of the robot and fill in the variable. You may need to play around with this value.
Task 1: Fill in the updateMotorSpeeds() function
Task 2a: Fill in the deadEnd() and intersection() functions
Task 2b: Fill in the driveToIntersectionCenter function
