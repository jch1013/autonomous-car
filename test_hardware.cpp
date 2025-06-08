// #include <Servo.h>

const int ledPin = 13;         // Built-in LED pin
int centerPos = 90;      // Center position for servo
int escPin = 3;
const int servoPin = 12;
const int frontEchoPin = 11;
const int frontTrigPin = 10;
const float soundSpeed = 0.034;



void setup() {
  pinMode(frontTrigPin, OUTPUT);
  pinMode(frontEchoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void sendUltrasonicPulse(int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

void loop() {
  // Turn LED on
  sendUltrasonicPulse(frontTrigPin);
  long duration = pulseIn(frontEchoPin, HIGH, 30000); // review this - pulseIn timeout probably can be much less than 1s
  float distance = duration * 0.034 / 2;

  // trigger light if within 20cm
  if (distance > 0 && distance < 20) {
    digitalWrite(ledPin, HIGH);
    esc.write(90);
  } else {
    digitalWrite(ledPin, LOW);
    esc.write(95);
  }

  delay(100);

}
