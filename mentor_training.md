# Mentor Training Day
This document contains the tasks required to complete on the mentor training day. 
The zumo library can be found at https://pololu.github.io/zumo-shield-arduino-library/.

## Part 1: Basic Programs
For each part, download and write your code in the the provided skeleton code in this repository under skeleton_code > day_1.

### Push Button
1. Wait for the button to be pressed, then blink the LED while the button is being held down.
Note for this part you will need to use the pinMode and digitalWrite functions, with the LED pin being 13. 

### Motor Control
1. Write a program to move the robot forwards at a constant velocity. Experiment with different values, with 400 being the maximum speed.
2. Modify the program to gradually accelerate the robot up to the maximum speed and maintain it for a few seconds. 
3. After that, decelerate the robot backwards from this max speed until it is stationary.
4. Turn the led on while the robot is accelerating, and turn it off when it is decelerating. 

### Turning
1. Move the robot forwards in a perfect circle so it loops back to its starting point (any size circle will do)
2. Once it reaches back, spin the robot slowly on the spot in the clockwise direction, then counterclockwise after it completes a full revolution

### Line Sensors
By looking underneath the robot, you will be able to see that there are 6 sensors at the front. These are used to detect black lines and are an important part of the main design challenge.
For this part, you will need to set up a few things before you can start. First, set up an array of length 6. This array will store the values of the individual line sensors, after reading them.
In your arduino setup, you will need to initialize the line sensors using the init() method in your line sensor object.  
The function readLine() is the main function you will be using for this whole part. It takes your line sensor array as an argument, and updates all the individual values of the sensors.
You can find more details about the line sensors under ZumoReflectanceSensorArray in the main library above. 

1. Using the serial monitor, test to see what values on the line sensors you get when the robot encounters a piece of black tape. For this part you don't need to drive the robot forward, just hold it and move it around.
2. Drive the robot forwards and make it stop when it sees a piece of black tape either side of it.
3. Drive the robot forwards and make it stop when it sees a black piece of tape in front of it

### Buzzer
1. Modify the above code to play a sound when it reaches a line

## Part 2: Design Challenge
For each part, download and write your code in the the provided skeleton code in this repository under skeleton_code > day_2 > oldMazeSolver.

### Some notes about the challenge
