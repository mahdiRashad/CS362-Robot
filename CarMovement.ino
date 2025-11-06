//Car movment code this code is responsible of the movment of the robot by depending on the resors readings from the sensors arduino
//Wire library used for communication between arduinos.
#include <Wire.h>

//Direction and speed pins for the the gear motors
int speedPinRight = 9;
int MotorDirPin1Right = 12;
int MotorDirPin2Right = 11;
int speedPinLeft = 6;
int MotorDirPin1Left = 7;
int MotorDirPin2Left = 8;

//Set the stop distence and turning delay
int STOP_CM = 30;
int REDIRECT_CM = 25;
int TURN_TIME = 350;
int GO_BACK_TIME = 500;

//Intialize the speed to half and full
//Half-speed forward
int half_speed = 128;
//Full-speed turns/back
int full_speed = 255;

//I²C address of your sensors arduino for communcation
const uint8_t SENSOR_ADDR = 0x10;

//Initlize driving states
enum State : int { DRIVE_FWD, AVOID_LEFT, AVOID_RIGHT, GO_BACK, HALT };
//Initilse the state to HALT
State state = HALT;
unsigned long stateStart = 0;
//update the state and time when witching between states
void setState(State s){ state = s; stateStartMs = millis(); }

//Motor helper functions, ahead, back, left turn, right turn. Also setSpeed and stop functions.
//Set the left side and right side speeds
void setSpeed(int left,int right){ 
  analogWrite(speedPinLeft,left); 
  analogWrite(speedPinRight,right); 
}

//Stop the robot
void stop(){ 
  digitalWrite(MotorDirPin1Right,LOW); 
  digitalWrite(MotorDirPin2Right,LOW); 
  digitalWrite(LeftDirPin1Motor,LOW); 
  digitalWrite(MotorDirPin2Left,LOW); 
  set_Motorspeed(0,0); 
}

//Move forward
void go_Advance(){
  digitalWrite(MotorDirPin1Right,HIGH); 
  digitalWrite(MotorDirPin2Right,LOW);
  digitalWrite(MotorDirPin1Left, HIGH); 
  digitalWrite(MotorDirPin2Left, LOW);
  setSpeed(half_speed, half_speed);
}

//Move backward
void go_Back(){
  digitalWrite(MotorDirPin1Right,LOW);  
  digitalWrite(MotorDirPin2Right,HIGH);
  digitalWrite(MotorDirPin1Left, LOW);  
  digitalWrite(MotorDirPin2Left, HIGH);
  setSpeed(half_speed, half_speed);
}

//Turn left
void turn_Left(){
  digitalWrite(MotorDirPin1Right,HIGH); 
  digitalWrite(MotorDirPin2Right,LOW);
  digitalWrite(MotorDirPin1Left, LOW);  
  digitalWrite(MotorDirPin2Left, HIGH);
  setSpeed(180, 180);
}

//Turn right
void turn_Right(){
  digitalWrite(MotorDirPin1Right,LOW);  
  digitalWrite(MotorDirPin2Right,HIGH);
  digitalWrite(MotorDirPin1Left, HIGH); 
  digitalWrite(MotorDirPin2Left, LOW);
  setSpeed(180, 180);
}

//Read 10 bytes: Left, FrontLeft, Front, FrontRight, Right from the sensors aruino
bool read5(uint16_t &Left,uint16_t &FrontLeft,uint16_t &Front,uint16_t &FrontRight,uint16_t &Right){
  Wire.requestFrom((int)SENSOR_ADDR, 10);
  if (Wire.available() < 10){
    return false;
  }
  auto rd16 = [](){ 
    uint8_t lo=Wire.read(); 
    uint8_t hi=Wire.read(); 
    return (uint16_t)lo | ((uint16_t)hi<<8); 
  };
  Left=rd16(); FrontLeft=rd16(); Front=rd16(); FrontRight=rd16(); Right=rd16();
  return true;
}

void setup(){
  //set pin modes
  pinMode(MotorDirPin1Right,OUTPUT); 
  pinMode(MotorDirPin2Right,OUTPUT); 
  pinMode(speedPinLeft,OUTPUT);
  pinMode(MotorDirPin1Left, OUTPUT); 
  pinMode(MotorDirPin2Left, OUTPUT); 
  pinMode(speedPinRight,OUTPUT);
  stop();

  //start the comunaction and serial monitor for live data
  Wire.begin();
  Serial.begin(9600);
  setState(DRIVE_FWD);
}

void loop(){
  //Make sure to repet every 100ms
  static unsigned long lastMs=0;
  if (millis()-lastMs >= 100){ lastMs = millis();

    //read the sensors data if there is not data halt and wait 100ms
    uint16_t L,FL,F,FR,R;
    if (!read5(L,FL,F,FR,R)){
      stop(); setState(HALT); Serial.println("I2C fail set state to HALT"); delay(100); return;
    }

    //Debug print the sensors values to the serial monitor
    Serial.print("Left:");Serial.print(L); 
    Serial.print(" FrontLeft:");Serial.print(FL);
    Serial.print(" Front:");Serial.print(F); 
    Serial.print(" FrontRight:");Serial.print(FR);
    Serial.print(" Right:");Serial.println(R);

    //Combine sectors
    uint16_t front_sector = F;
    uint16_t frontLeftRight = min(FL, FR);
    uint16_t left_space   = max(L, FL);
    uint16_t right_space  = max(R, FR);

    switch (state){
      case DRIVE_FWD:
        if (front_sector < STOP_CM || frontLeftRight < REDIRECT_CM){
          stop(); delay(40);
          //choose side with more space
          if ((left_space < 20 && right_space < 20) || (FL < 15 && FR < 15)){
            setState(GO_BACK);
          }
          else if (left_space >= right_space && FL > 15) {
            setState(AVOID_LEFT);
          }
          else if(left_space < right_space && FR > 15){
            setState(AVOID_RIGHT);
          }
          else{
            setState(GO_BACK);
          }
        } else {
          go_Advance();
        }
        break;

      case AVOID_LEFT:
        turn_Left();
        if (millis() - stateStartMs > TURN_TIME) setState(DRIVE_FWD);
        break;

      case AVOID_RIGHT:
        turn_Right();
        if (millis() - stateStartMs > TURN_TIME) setState(DRIVE_FWD);
        break;

      case GO_BACK:
        go_Back();
        if (millis() - stateStartMs > GO_BACK_TIME) setState(DRIVE_FWD);
        break;

      case HALT:
      default:
        stop();
        if (front_sector >= STOP_CM) setState(DRIVE_FWD);
        break;
    }
  }
}
