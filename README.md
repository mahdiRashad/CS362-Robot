Team Information:
Mahdi Yahya			myahy3	myahy3@uic.edu
Mohammed Kamil		mkami3	mkami3@uic.edu
Qasim Simba			qsimb2		qsimb2@uic.edu
Project Title:
“Autonomous Floor Cleaning Robot”
Group Number:
Project Ideas:
Our project will focus on designing and building an autonomous floor cleaning robot, a car-like design with 4 wheels and a double layered chassis that holds all the components together. The robot will be capable of cleaning indoor spaces using a mop attached to a motor. The robot will wet, scrub, and then dry the floor in a single pass so the surface is walkable shortly after cleaning. The robot will drive itself using sensors, so the system will be able to move independently and avoid collisions.
Detailed Project Ideas:
Overall Description of Project Idea
The Autonomous Floor Cleaning Robot will function as a mobile cleaning system that can automatically detect obstacles using multiple ultrasonic sensors and navigate across a flat indoor surface. The robot will consist of a double layered chassis mounted on four wheels (each wheel is connected to a gear motor) and a motor driver module that controls the movement of the robot. An arduino and a power supply will be needed to direct the robot and power the movement system.
The robot will use ultrasonic sensors to detect obstacles, a total of 5 ultrasonic sensors will be used (on the front, left, right, front-left, and front-right sides). An arduino will be responsible for reading the input from the sensors, deciding which direction the robot must follow to avoid collection, and sending signals to the movement arduino. 
A mob attached to a motor mounted on the bottom of the robot will be used to clean the floor. Also, a water pump will spray water on the ground for better cleaning and a fan attached at the end of the robot to dry the floor. All of these cleaning parts will be controlled by an arduino and a separate power supply will be used, as well as a small water tank.
Initial Project Design stating how Multiple Arduinos will be used
This separation of tasks into categories ensures efficient distribution of processing load and makes troubleshooting or system expansion easier:
1- Arduino 1 (Movement Controller): will control the movement and speed of the robot motors (forward, backward, and turning movements based on sensor inputs).
2-Arduino 2 (Obstacle Detection & Interface): will be responsible for the sensors (ultrasonic sensors identify obstacles, calculate distances, and guide safe navigation to the Movement controller), also, LEDs and buzzer will be used to notify people when the robot get close to an obstacle.
3-Arduino 3 (Cleaning): will be responsible for cleaning the floor, spraying the water, and drying the floor using a fan and a mob attached to a motor.
Expected Plan for Use and Communication between the multiple Arduinos
Arduino 2 the one responsible for the sensors will send the safe navigation to Arduino 1 the movement controller. The movement controller will then move the robot to the direction. We will use serial communication: SDA → SDA, SCL → SCL, GND → GND. 
Initial Project Design stating Expected Inputs/Outputs
Movement arduino: 
4x Gear motor: move the robot.
Motor driver module: control the Gear motor.
Battery pack with 2x 18650 Battery 3.2V 3400mah: power the movement system
Cleaning arduino:
Fan: drying the floor.
Pump: wetting the floor.
MG996R Servo Motor: attached to the mob for efficient cleaning coverage.
Battery pack with 2x 18650 Battery 3.2V 3400mah: power the cleaning system.
Voltage Regulator: MG996R Servo Motor requires different voltages.
Sensors arduino:
Ultrasonic sensor: 5 of them to avoid collisions.
LED strip: lights up when the robot gets close to an obstacle.
Buzzer: generate sound when the robot gets close to an obstacle.
Description of the original work
All of the code used for each arduino will be implemented by the group members. We combined different components into one robot that is capable of moving independently and cleaning the floor.
