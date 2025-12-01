#include <Servo.h>

// ======== SERVO SETUP (always running) ========
Servo brushServo;
const int SERVO_PIN = 7;

// Continuous rotation servo values:
const int SERVO_RUN = 180;   // Full speed clockwise


// ======== PUMP SETUP (TA6586) ========
const int motorBI = 9;   // Backward input
const int motorFI = 10;  // Forward input

// Spray timing
const unsigned long SPRAY_PULSE_MS    = 45;       // 0.25 sec spray (few drops)
const unsigned long SPRAY_INTERVAL_MS = 30000UL;   // 30 seconds interval

bool pumping = false;
unsigned long sprayStartTime = 0;
unsigned long lastSprayTime  = 0;


void setup() {
  // ----- Servo (cleaning brush) -----
  brushServo.attach(SERVO_PIN);
  brushServo.write(SERVO_RUN);  // Keep servo always running

  // ----- Pump driver pins -----
  pinMode(motorBI, OUTPUT);
  pinMode(motorFI, OUTPUT);

  // Ensure pump is OFF at start
  digitalWrite(motorBI, LOW);
  digitalWrite(motorFI, LOW);

  // Start the timer
  lastSprayTime = millis();
}


void loop() {
  unsigned long now = millis();

  // ======== Pump running phase ========
  if (pumping) {
    if (now - sprayStartTime >= SPRAY_PULSE_MS) {
      // Stop pump
      digitalWrite(motorFI, LOW);
      digitalWrite(motorBI, LOW);

      pumping = false;
      lastSprayTime = now;
    }
    return;  // don’t start a new spray while pumping
  }


  // ======== Time to start a new spray? ========
  if (now - lastSprayTime >= SPRAY_INTERVAL_MS) {
    // Turn pump ON (forward direction)
    digitalWrite(motorBI, LOW);
    digitalWrite(motorFI, HIGH);

    pumping = true;
    sprayStartTime = now;
  }
}
