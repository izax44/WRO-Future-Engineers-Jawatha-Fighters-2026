#include <NewPing.h>

NewPing us1(2, 1, 200);    // Left:   trig D2,  echo D1
NewPing us2(4, A4, 200);   // Center: trig D4,  echo A4
NewPing us3(0, A2, 200);  // Right:  trig D0, echo A2

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned int d1 = us1.ping_cm();
  unsigned int d2 = us2.ping_cm();
  unsigned int d3 = us3.ping_cm();

  Serial.print("L:"); Serial.print(d1);
  Serial.print("cm  C:"); Serial.print(d2);
  Serial.print("cm  R:"); Serial.print(d3);
  Serial.println("cm");
  delay(150);
}
