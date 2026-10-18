#include <Wire.h>

const int MPU = 0x68;  // I2C address (AD0 = GND)
int16_t ax, ay, az, gx, gy, gz;

void setup() {
  Serial.begin(9600);
  Wire.begin();              // XMotion V3: SDA = A4, SCL = A5
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); Wire.write(0);   // wake up
  Wire.endTransmission(true);
  delay(100);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read();          // skip temp
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();

  float pitch = atan2(ay, az) * 180 / PI;   // tilt angles
  float roll  = atan2(ax, az) * 180 / PI;

  Serial.print("Pitch:"); Serial.print(pitch, 1);
  Serial.print("  Roll:"); Serial.print(roll, 1);
  Serial.print("  GZ:");  Serial.println(gz);
  delay(200);
}
