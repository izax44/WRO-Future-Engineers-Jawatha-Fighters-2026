
#include <Wire.h>
#include <Servo.h>


const int MOTOR_PWM = 10;
const int MOTOR_DIR = 12;
const int SERVO_PIN = 4;

const int TRIG_L = 5;
const int ECHO_L = 6;
const int TRIG_C = 0;
const int ECHO_C = 1;
const int TRIG_R = A2;
const int ECHO_R = A1;

const int SERVO_CENTER = 90;
const int SERVO_LEFT   = 60;
const int SERVO_RIGHT  = 120;

const int CRUISE_SPEED = 100; 
const int TURN_SPEED   = 80;

const int CORNER_DISTANCE = 45;      
const float TURN_ANGLE  = 90.0;      
const float TURN_MARGIN = 8.0;       
const unsigned long TURN_MAX_MS = 3000;  

const float WALL_TARGET = 25.0;   
const float KP = 0.8;            
const float KP_HEADING = 1.0;    
const int MAX_TRIM = 20;         

const int TOTAL_CORNERS = 12;    
const unsigned long FINISH_MS = 1500;  


const int MPU_ADDR = 0x68;
const float GYRO_SIGN = 1.0;     

Servo steering;

enum State { DRIVE, TURN, STOPPED };
State state = DRIVE;  

int dir = 0;            
int corners = 0;
unsigned long turnStart = 0;
unsigned long finishStart = 0;

float heading = 0;       
float targetHeading = 0;  
float gyroBias = 0;
unsigned long lastImuMicros = 0;


int16_t readGyroZRaw() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x47);                   
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 2);
  int hi = Wire.read();
  int lo = Wire.read();
  return (int16_t)((hi << 8) | lo);
}

void initImu() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);                   
  Wire.write(0);
  Wire.endTransmission();
  delay(100);


  long sum = 0;
  for (int i = 0; i < 200; i++) {
    sum += readGyroZRaw();
    delay(5);
  }
  gyroBias = sum / 200.0;
  lastImuMicros = micros();
}


void updateHeading() {
  unsigned long now = micros();
  float dt = (now - lastImuMicros) / 1000000.0;
  lastImuMicros = now;

  float rate = GYRO_SIGN * (readGyroZRaw() - gyroBias) / 131.0;  
  if (fabs(rate) < 0.3) rate = 0;                               
  heading += rate * dt;
}


float readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 18000);
  if (duration == 0) return -1;  
  return duration * 0.0343 / 2.0;
}

void setMotor(int speedValue) {
  digitalWrite(MOTOR_DIR, HIGH);  
  analogWrite(MOTOR_PWM, constrain(speedValue, 0, 255));
}

void stopMotor() {
  analogWrite(MOTOR_PWM, 0);
}

void setSteering(int angle) {
  steering.write(constrain(angle, SERVO_LEFT, SERVO_RIGHT));
}


void followWall(float left, float right) {
  float wall = (dir == 1) ? left : right;

  int angle = SERVO_CENTER +
              constrain((int)(KP_HEADING * (targetHeading - heading)), -MAX_TRIM, MAX_TRIM);

  if (dir != 0 && wall >= 0) {
    angle -= dir * constrain((int)(KP * (wall - WALL_TARGET)), -MAX_TRIM, MAX_TRIM);
  }

  setSteering(angle);
}


void setup() {
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(TRIG_C, OUTPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);

  steering.attach(SERVO_PIN);
  setSteering(SERVO_CENTER);
  stopMotor();

  initImu();  
}


void loop() {
  updateHeading();

  if (state == STOPPED) {
    stopMotor();
    setSteering(SERVO_CENTER);
    return;
  }


  if (state == TURN) {
    setSteering(dir == 1 ? SERVO_RIGHT : SERVO_LEFT);
    setMotor(TURN_SPEED);

    bool reached = dir * (targetHeading - heading) <= TURN_MARGIN;

    if (reached || millis() - turnStart >= TURN_MAX_MS) {
      corners++;
      state = DRIVE;
      if (corners == TOTAL_CORNERS) finishStart = millis();
    }
    return;
  }

  float left  = readDistance(TRIG_L, ECHO_L);
  float front = readDistance(TRIG_C, ECHO_C);
  float right = readDistance(TRIG_R, ECHO_R);

  if (front < 0) {
    stopMotor();
    return;
  }


  if (corners >= TOTAL_CORNERS) {
    if (millis() - finishStart >= FINISH_MS) state = STOPPED;
  }

  else if (front < CORNER_DISTANCE) {
    if (dir == 0) dir = (left > right) ? -1 : 1; 
    targetHeading += dir * TURN_ANGLE;          
    turnStart = millis();
    state = TURN;
    return;
  }

  followWall(left, right);
  setMotor(CRUISE_SPEED);
}
