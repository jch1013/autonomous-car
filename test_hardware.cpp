#include <Servo.h>

const int ledPin = 13;
const int escPin = 3;
const int frontEchoPin = 11;
const int frontTrigPin = 10;
const float soundSpeed = 0.034; // cm/us

Servo esc;

// Pulse timing settings
const int forwardSpeed = 1575;      // ESC pulse for forward
const int neutralSpeed = 1500;      // ESC pulse for neutral
const unsigned long onTime = 100;   // ms motor ON
const unsigned long offTime = 200;  // ms motor OFF

// State variables for non-blocking timing
bool motorOn = false;
unsigned long lastSwitchTime = 0;

void setup() {
  pinMode(frontTrigPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  esc.attach(escPin);
  esc.writeMicroseconds(neutralSpeed); // Arm ESC
  delay(2000); // Wait for ESC to arm
}

void sendUltrasonicPulse(int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

float readDistance() {
  sendUltrasonicPulse(frontTrigPin);
  long duration = pulseIn(frontEchoPin, HIGH, 30000); // 30ms timeout
  float distance = duration * soundSpeed / 2.0;
  return distance;
}

// Non-blocking drive forward function
void driveForward() {
  unsigned long currentMillis = millis();

  if (motorOn) {
    if (currentMillis - lastSwitchTime >= onTime) {
      esc.writeMicroseconds(neutralSpeed); // Switch to neutral
      motorOn = false;
      lastSwitchTime = currentMillis;
    }
    // else: keep running forward
  } else {
    if (currentMillis - lastSwitchTime >= offTime) {
      esc.writeMicroseconds(forwardSpeed); // Switch to forward
      motorOn = true;
      lastSwitchTime = currentMillis;
    }
    // else: keep in neutral
  }
}

void loop() {
  float distance = readDistance();

  if (distance > 0 && distance < 20) {
    // Obstacle detected: stop and turn LED on
    digitalWrite(ledPin, HIGH);
    esc.writeMicroseconds(neutralSpeed); // Stop motor
    motorOn = false; // Reset state so next pulse starts with ON
    lastSwitchTime = millis();
  } else {
    // No obstacle: pulse forward and turn LED off
    digitalWrite(ledPin, LOW);
    driveForward();
  }
}
