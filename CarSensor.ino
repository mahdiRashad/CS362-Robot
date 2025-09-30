#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "pitches.h"

/* ---------- Pin map (edit if you rewired) ---------- */
const uint8_t l_trig  = 5,  l_echo  = 3;   // Left
const uint8_t fl_trig = 6,  fl_echo = 7;   // Front-Left
const uint8_t f_trig  = 2,  f_echo  = 4;   // Front (center)
const uint8_t fr_trig = 9,  fr_echo = 10;  // Front-Right
const uint8_t r_trig  = 12, r_echo  = 11;  // Right


const int LED_PIN = 13;
const int LED_COUNT = 8;
const int buzzerPin = 8;
bool color_state = false;
static unsigned long t = 0;
static unsigned long t2 = 0;

/* ---------- I2C address ---------- */
const uint8_t I2C_ADDR = 0x10;

/* ---------- Latest distances (cm) ---------- */
volatile uint16_t dL = 400, dFL = 400, dF = 400, dFR = 400, dR = 400;


/* ---------- Helpers ---------- */
// Convert echo time (µs) to cm; clamp 2..400; 0 (timeout) -> 400
static inline uint16_t echoToCm(unsigned long us) {
  if (us == 0) return 400;          // timeout -> treat as far
  float cm = us / 58.0f;            // 343 m/s, round-trip
  if (cm < 2)   cm = 2;
  if (cm > 400) cm = 400;
  return (uint16_t)cm;
}

static inline unsigned long pingOnce(uint8_t trig, uint8_t echo, unsigned long timeoutUs = 30000UL) {
  digitalWrite(trig, LOW);  delayMicroseconds(3);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, timeoutUs);    // µs, 0 if timeout
}

// median of 3 pings for stability; ~25 ms spacing minimizes cross-talk
uint16_t readDistanceCm(uint8_t trig, uint8_t echo) {
  unsigned long a = pingOnce(trig, echo); delay(25);
  unsigned long b = pingOnce(trig, echo); delay(25);
  unsigned long c = pingOnce(trig, echo);

  // sort so b is median
  if (a > b) { unsigned long t=a; a=b; b=t; }
  if (b > c) { unsigned long t=b; b=c; c=t; }
  if (a > b) { unsigned long t=a; a=b; b=t; }

  return echoToCm(b);
}

/* ---------- I2C reply: 10 bytes L,FL,F,FR,R (uint16 LE) ---------- */
void onI2CRequest() {
  uint16_t L, FL, F, FR, R;

  // atomic copy of 16-bit values
  noInterrupts();
  L  = dL;  FL = dFL;  F  = dF;  FR = dFR;  R  = dR;
  interrupts();

  uint8_t p[10] = {
    (uint8_t)(L & 0xFF),  (uint8_t)(L >> 8),
    (uint8_t)(FL & 0xFF), (uint8_t)(FL >> 8),
    (uint8_t)(F & 0xFF),  (uint8_t)(F >> 8),
    (uint8_t)(FR & 0xFF), (uint8_t)(FR >> 8),
    (uint8_t)(R & 0xFF),  (uint8_t)(R >> 8)
  };
  Wire.write(p, sizeof(p));
}

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(l_trig,  OUTPUT); pinMode(l_echo,  INPUT);
  pinMode(fl_trig, OUTPUT); pinMode(fl_echo, INPUT);
  pinMode(f_trig,  OUTPUT); pinMode(f_echo,  INPUT);
  pinMode(fr_trig, OUTPUT); pinMode(fr_echo, INPUT);
  pinMode(r_trig,  OUTPUT); pinMode(r_echo,  INPUT);

  Wire.begin(I2C_ADDR);          // I2C slave
  Wire.onRequest(onI2CRequest);  // reply handler

  pinMode(buzzerPin, OUTPUT);
  strip.begin();
  strip.setBrightness(150); 
  strip.show(); 

  Serial.begin(9600);            // optional debug
}

/* Read all sensors in a left->right sweep every ~150–200 ms */
void loop() {
  int milli = millis();
  if (millis() - t >= 100) {     // adjust if you want faster/slower updates
    t = millis();

    uint16_t L  = readDistanceCm(l_trig,  l_echo);
    uint16_t FL = readDistanceCm(fl_trig, fl_echo);
    uint16_t F  = readDistanceCm(f_trig,  f_echo);
    uint16_t FR = readDistanceCm(fr_trig, fr_echo);
    uint16_t R  = readDistanceCm(r_trig,  r_echo);

    dL = L; dFL = FL; dF = F; dFR = FR; dR = R;
    
    if(dFL <= 20 || dF <= 20 || dFR <= 20){
      // Red if object closer than 20 cm
        int milli2 = millis();

        if(milli2 - t2 >= 100){
          t2 = milli2;
          color_state = !color_state;
        }

        if(color_state == true){
          strip.fill(strip.Color(255, 0, 0));  
          tone(buzzerPin, NOTE_C4, 100);
        } else if (color_state == false){
          strip.fill(strip.Color(0, 0, 255));  
          tone(buzzerPin, NOTE_C4, 100);
        }

      } else {
        strip.fill(strip.Color(255, 255, 255));  
        digitalWrite(buzzerPin, LOW);  // buzzer OFF
      }

      strip.show();

    // Debug prints
    Serial.print("L:");  Serial.print(L);
    Serial.print(" FL:");Serial.print(FL);
    Serial.print(" F:"); Serial.print(F);
    Serial.print(" FR:");Serial.print(FR);
    Serial.print(" R:"); Serial.println(R);
  }
}