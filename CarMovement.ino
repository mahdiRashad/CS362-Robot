// ===== Motor Arduino (I2C Master, 5-sensor avoidance) =====
#include <Wire.h>

// ---- Motor pins
#define speedPinR 9
#define RightMotorDirPin1 12
#define RightMotorDirPin2 11
#define speedPinL 6
#define LeftMotorDirPin1 7
#define LeftMotorDirPin2 8

const uint8_t SENSOR_ADDR = 0x10;

// ---- Speeds
const uint8_t SPEED_FWD_L  = 128;   // half-speed forward
const uint8_t SPEED_FWD_R  = 128;
const uint8_t SPEED_FULL_L = 255;   // full-speed turns/back
const uint8_t SPEED_FULL_R = 255;

// ---- Thresholds
const uint16_t FRONT_STOP_CM = 30;  // when to avoid
const uint16_t TURN_TIME_MS  = 250;

// ---- State machine
enum State : uint8_t { DRIVE_FWD, AVOID_LEFT, AVOID_RIGHT, HALT };
State state = HALT;
unsigned long stateStartMs = 0;
void setState(State s){ state = s; stateStartMs = millis(); }

// ---- Motor helpers
void set_Motorspeed(int l,int r){ analogWrite(speedPinL,constrain(l,0,255)); analogWrite(speedPinR,constrain(r,0,255)); }
void stop_Stop(){ digitalWrite(RightMotorDirPin1,LOW); digitalWrite(RightMotorDirPin2,LOW); digitalWrite(LeftMotorDirPin1,LOW); digitalWrite(LeftMotorDirPin2,LOW); set_Motorspeed(0,0); }
void go_Advance(){
  digitalWrite(RightMotorDirPin1,HIGH); digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1, HIGH); digitalWrite(LeftMotorDirPin2, LOW);
  set_Motorspeed(128, 128);
}
void go_Back(){
  digitalWrite(RightMotorDirPin1,LOW);  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);  digitalWrite(LeftMotorDirPin2, HIGH);
  set_Motorspeed(SPEED_FULL_L, SPEED_FULL_R);
}
void turn_Left_inPlace(){
  digitalWrite(RightMotorDirPin1,HIGH); digitalWrite(RightMotorDirPin2,LOW);
  digitalWrite(LeftMotorDirPin1, LOW);  digitalWrite(LeftMotorDirPin2, HIGH);
  set_Motorspeed(175, 175);
}
void turn_Right_inPlace(){
  digitalWrite(RightMotorDirPin1,LOW);  digitalWrite(RightMotorDirPin2,HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH); digitalWrite(LeftMotorDirPin2, LOW);
  set_Motorspeed(175, 175);
}
void init_GPIO(){
  pinMode(RightMotorDirPin1,OUTPUT); pinMode(RightMotorDirPin2,OUTPUT); pinMode(speedPinL,OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT); pinMode(LeftMotorDirPin2, OUTPUT); pinMode(speedPinR,OUTPUT);
  stop_Stop();
}

// ---- Read 10 bytes: L, FL, F, FR, R (uint16 LE)
bool read5(uint16_t &L,uint16_t &FL,uint16_t &F,uint16_t &FR,uint16_t &R){
  Wire.requestFrom((int)SENSOR_ADDR, 10);
  if (Wire.available() < 10) return false;
  auto rd16 = [](){ uint8_t lo=Wire.read(); uint8_t hi=Wire.read(); return (uint16_t)lo | ((uint16_t)hi<<8); };
  L=rd16(); FL=rd16(); F=rd16(); FR=rd16(); R=rd16();
  return true;
}

void setup(){
  init_GPIO();
  Wire.begin();
  Serial.begin(9600);
  setState(DRIVE_FWD);
}

void loop(){
  static unsigned long lastMs=0;
  if (millis()-lastMs >= 100){ lastMs = millis();

    uint16_t L,FL,F,FR,R;
    if (!read5(L,FL,F,FR,R)){
      stop_Stop(); setState(HALT); Serial.println("I2C fail -> HALT"); delay(80); return;
    }
    // Debug
    Serial.print(" L:");Serial.print(L); Serial.print(" FL:");Serial.print(FL);
    Serial.print(" F:");Serial.print(F); Serial.print(" FR:");Serial.print(FR);
    Serial.print(" R:");Serial.println(R);

    // Combine sectors
    uint16_t front_sector = min(F, min(FL, FR));   // anything close in front counts
    uint16_t left_space   = max(L, FL);
    uint16_t right_space  = max(R, FR);

    switch (state){
      case DRIVE_FWD:
        if (front_sector < FRONT_STOP_CM){
          stop_Stop(); delay(40);
          // choose side with more space
          if (left_space > right_space) setState(AVOID_LEFT);
          else                          setState(AVOID_RIGHT);
        } else {
          go_Advance();
        }
        break;

      case AVOID_LEFT:
        turn_Left_inPlace();
        if (millis() - stateStartMs > TURN_TIME_MS) setState(DRIVE_FWD);
        break;

      case AVOID_RIGHT:
        turn_Right_inPlace();
        if (millis() - stateStartMs > TURN_TIME_MS) setState(DRIVE_FWD);
        break;

      case HALT:
      default:
        stop_Stop();
        if (front_sector >= FRONT_STOP_CM) setState(DRIVE_FWD);
        break;
    }
  }
}
