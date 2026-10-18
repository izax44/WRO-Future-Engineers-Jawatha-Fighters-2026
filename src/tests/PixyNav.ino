#include <xmotion.h>
#include <Pixy2.h>
#include <Servo.h>

Pixy2 pixy;
Servo steering;

// TUNING
const int SERVO_PIN  = 9;
const int CENTER = 90, STEER_LEFT = 60, STEER_RIGHT = 120;
const int DRIVE_SPEED = 60;
const int MIN_AREA    = 300;    // ignore small/noise blocks
const int ACT_TIME_MS = 60;     // action burst per loop cycle

// Pixy signatures: 1=RED 2=GREEN 3=BLUE 4=ORANGE
const int SIG_RED = 1, SIG_GREEN = 2, SIG_BLUE = 3, SIG_ORANGE = 4;
// ----------------------------

void setup() {
  steering.attach(SERVO_PIN);
  steering.write(CENTER);
  xmotion.StopMotors(100);
  pixy.init();
  Serial.begin(9600);
}

void loop() {
  // Scan all blocks, keep largest per color, leftmost x for line colors
  uint32_t areaR = 0, areaG = 0, areaB = 0, areaO = 0;
  int16_t xB = 400, xO = 400;   // 400 = "not seen"

  pixy.ccc.getBlocks();
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    uint32_t a = (uint32_t)pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
    if (a < MIN_AREA) continue;
    switch (pixy.ccc.blocks[i].m_signature) {
      case SIG_RED:    if (a > areaR) areaR = a; break;
      case SIG_GREEN:  if (a > areaG) areaG = a; break;
      case SIG_BLUE:
        if (a > areaB) areaB = a;
        if (pixy.ccc.blocks[i].m_x < xB) xB = pixy.ccc.blocks[i].m_x;
        break;
      case SIG_ORANGE:
        if (a > areaO) areaO = a;
        if (pixy.ccc.blocks[i].m_x < xO) xO = pixy.ccc.blocks[i].m_x;
        break;
    }
  }

  // avoid Red/Green obstacle blocks
  if (areaR > 0 || areaG > 0) {
    if (areaR >= areaG && areaR > 0) {
      Serial.println("RED block -> steer RIGHT");
      steering.write(STEER_RIGHT);
    } else {
      Serial.println("GREEN block -> steer LEFT");
      steering.write(STEER_LEFT);
    }
    xmotion.Forward(DRIVE_SPEED, ACT_TIME_MS);   // drive past the block
    steering.write(CENTER);
  }

  // end-of-line marker (Blue/Orange)
  else if (areaB > 0 || areaO > 0) {
    if (areaB > 0 && (areaO == 0 || xB < xO)) {
      Serial.println("BLUE first -> turn LEFT (CCW)");
      steering.write(STEER_LEFT);
    } else {
      Serial.println("ORANGE first -> turn RIGHT (CW)");
      steering.write(STEER_RIGHT);
    }
    xmotion.Forward(DRIVE_SPEED, ACT_TIME_MS);
    steering.write(CENTER);
  }

  // nothing seen -> drive straight
  else {
    steering.write(CENTER);
    xmotion.Forward(DRIVE_SPEED, ACT_TIME_MS);
  }
}
