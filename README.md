Team Information:

Mahdi Yahya			myahy3	myahy3@uic.edu
Mohammed Kamil		mkami3	mkami3@uic.edu
Qasim Simba			qsimb2		qsimb2@uic.edu

Project Title:

“Autonomous Floor Cleaning Robot”

Project Ideas:

Our project will focus on designing and building an autonomous floor cleaning robot, a car-like design with 4 wheels and a double layered chassis that holds all the components together. The robot will be capable of cleaning indoor spaces using a mop attached to a motor. The robot will wet, scrub, and then dry the floor in a single pass so the surface is walkable shortly after cleaning. The robot will drive itself using sensors, so the system will be able to move independently and avoid collisions.

Detailed Project Ideas:

Overall Description of Project Idea

The Autonomous Floor Cleaning Robot will function as a mobile cleaning system that can automatically detect obstacles using multiple ultrasonic sensors and navigate across a flat indoor surface. The robot will consist of a double layered chassis mounted on four wheels (each wheel is connected to a gear motor) and a motor driver module that controls the movement of the robot. An arduino and a power supply will be needed to direct the robot and power the movement system.
The robot will use ultrasonic sensors to detect obstacles, a total of 5 ultrasonic sensors will be used (on the front, left, right, front-left, and front-right sides). An arduino will be responsible for reading the input from the sensors, deciding which direction the robot must follow to avoid collection, and sending signals to the movement arduino. 
A mob attached to a motor mounted on the bottom of the robot will be used to clean the floor. Also, a water pump will spray water on the ground for better cleaning and a fan attached at the end of the robot to dry the floor. All of these cleaning parts will be controlled by an arduino and a separate power supply will be used, as well as a small water tank.
Initial Project Design stating how Multiple Arduinos will be used
This separation of tasks into categories ensures efficient distribution of processing load and makes troubleshooting or system expansion easier:
1- Arduino 1 (Movement Controller): will control the movement and speed of the robot motors (forward, backward, and turning movements based on sensor inputs). The robot will move autonomously using the sensor's data input form the sensors arduino.
2-Arduino 2 (Obstacle Detection & Interface): will be responsible for the sensors (ultrasonic sensors identify obstacles, calculate distances, and guide safe navigation to the Movement controller), also, LEDs and buzzer will be used to notify people when the robot get close to an obstacle. This arduino will send the sensor data to the other arduinos using I2C communication.
3-Arduino 3 (Cleaning): will be responsible for cleaning the floor, spraying the water, and drying the floor using a fan and a mob attached to a motor. A tank will be used as a water reservoir and fan to dry the floor. A pump will spray the water on the floor.

Expected Plan for Use and Communication between the multiple Arduinos

Arduino 2 the one responsible for the sensors will send the safe navigation to Arduino 1 the movement controller. The movement controller will then move the robot to the direction. We will use serial communication: SDA → SDA, SCL → SCL, GND → GND. We are also going to use the wire library for easier sending and receiving of data.
Initial Project Design stating Expected Inputs/Outputs

Movement arduino: 

4x Gear motor: move the robot.
Motor driver module (OSOYOO model X): control the Gear motor.
Battery pack with 2x 18650 Battery 3.2V 3400mah: power the movement system
Two layer car chassis: hold all the components together.
Barrel power connect: Power arduinos.

Cleaning arduino:

Fan: drying the floor.
Pump: wetting the floor.
MG996R Servo Motor: attached to the mob for efficient cleaning coverage.
Battery pack with 2x 18650 Battery 3.2V 3400mah: power the cleaning system.
Voltage Regulator: MG996R Servo Motor requires different voltages.
Tank: For water reservoir. 

Sensors arduino:

Breadboard: Connect the ultrasonic sensors, LED strip, and buzzer.
Ultrasonic sensor: 5 of them to avoid collisions.
LED strip: lights up when the robot gets close to an obstacle.
Buzzer: generate sound when the robot gets close to an obstacle.

Description of the original work

All of the code used for each arduino will be implemented by the group members. We combined different components into one robot that is capable of moving independently and cleaning the floor. We are going to use wire library for communication between our arduinos. Furthermore, the design of parts and their location on the robot are all made by the group members.
How to build your project
Movement part:
Start by mounting the four motors to the metal chassis of the robot using the screws. Connect the wheels to the motors and use screws to tighten them. Mount the motor driver module on the chassis using screws and connect the connector from each motor to the motor drivers module. Mount the battery pack on the chassis using screws. Mount the movement arduino on the chassis using screws. Connect the first power cable to the motor driver module and the second one (barrel connector) to the arduino. Connect the 7 wires between the arduino and motor driver module: ENA→D9, ENB→D6, IN1→D12, IN2→D11, IN3→D7, IN4→D8. Right code to control the movement of the robot.
Sensors part:
Cleaning part:
How to use the robot
Before powering the robot make sure that water tank has water in it and the mob is washed clean and attached to the servo motor beneath the robot. Then make sure the batteries are charged and are in the battery pack. Turn on the robot using the two power buttons on the battery pack. The robot is to be used indoors on hardwood or ceramic floors. The robot will make sound and light red/blue when it is close to an obstacle after which it will change direction.

References:

https://osoyoo.com/2024/06/12/osoyoo-universal-robot-platform-for-uno/
https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
https://docs.arduino.cc/language-reference/en/functions/communication/wire/
https://projecthub.arduino.cc/lakshyajhalani56/smart-irrigation-soil-moisture-sensor-arduino-water-pump-c8be68
https://docs.arduino.cc/learn/electronics/servo-motors/
https://docs.arduino.cc/language-reference/en/functions/communication/wire/
https://projecthub.arduino.cc/SURYATEJA/use-a-buzzer-module-piezo-speaker-using-arduino-uno-cf4191
https://projecthub.arduino.cc/semsemharaz/interfacing-rgb-led-with-arduino-b59902

Diagrams:

1-

<img width="1597" height="490" alt="Screenshot 2025-11-05 at 11 10 45 PM" src="https://github.com/user-attachments/assets/2b882a67-f12e-4de8-a38d-2520c72d4c0a" />

2-

<img width="808" height="491" alt="Screenshot 2025-11-05 at 11 10 53 PM" src="https://github.com/user-attachments/assets/0965944a-8251-4595-9c0b-ebe1d6317819" />

3-

<img width="928" height="592" alt="Screenshot 2025-11-05 at 11 10 59 PM" src="https://github.com/user-attachments/assets/e2e882ca-a8b4-478e-8f53-d03c04067f79" />
