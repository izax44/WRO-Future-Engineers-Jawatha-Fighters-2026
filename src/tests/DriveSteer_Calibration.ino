#include <xmotion.h>
#include <Servo.h>

Servo steering;
const int SERVO_PIN = 2;

// TUNING
const int CENTER      = 90;   // straight ahead
const int STEER_LEFT  = 60;   // full left
const int STEER_RIGHT = 120;  // full right
const int DRIVE_SPEED = 70;   // 0-100

void setup() {
  steering.attach(SERVO_PIN);
  steering.write(CENTER);
  xmotion.StopMotors(100);
  delay(1000);
}

void loop() {
  // Steering servo sweep
  for (int a = STEER_LEFT; a <= STEER_RIGHT; a += 5) {
    steering.write(a); delay(80);
  }
  for (int a = STEER_RIGHT; a >= STEER_LEFT; a -= 5) {
    steering.write(a); delay(80);
  }
  steering.write(CENTER);
  delay(300);

  // Drive motor test
  xmotion.Forward(DRIVE_SPEED, 1000);    // drive forward 1 sec
  delay(500);
  xmotion.Backward(DRIVE_SPEED, 1000);   // reverse 1 sec
  delay(500);

  // Turn test
  steering.write(STEER_LEFT);
  xmotion.Forward(DRIVE_SPEED, 800);     // turn left arc
  steering.write(CENTER);
  delay(300);

  steering.write(STEER_RIGHT);
  xmotion.Forward(DRIVE_SPEED, 800);     // turn right arc
  steering.write(CENTER);
  delay(300);

  xmotion.StopMotors(100);
  delay(2000);
}
