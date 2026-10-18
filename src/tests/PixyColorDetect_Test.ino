#include <Pixy2.h>

Pixy2 pixy;

// Pixy signatures: 1=RED 2=GREEN 3=BLUE 4=ORANGE
const char* colorName(int sig) {
  switch (sig) {
    case 1: return "RED";
    case 2: return "GREEN";
    case 3: return "BLUE";
    case 4: return "ORANGE";
    default: return "UNKNOWN";
  }
}

void setup() {
  Serial.begin(9600);
  pixy.init();
  Serial.println("Color | X | Y | W | H | Area");
}

void loop() {
  pixy.ccc.getBlocks();
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    int sig = pixy.ccc.blocks[i].m_signature;
    uint32_t area = (uint32_t)pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;

    Serial.print(colorName(sig)); Serial.print(" | ");
    Serial.print(pixy.ccc.blocks[i].m_x); Serial.print(" | ");
    Serial.print(pixy.ccc.blocks[i].m_y); Serial.print(" | ");
    Serial.print(pixy.ccc.blocks[i].m_width); Serial.print(" | ");
    Serial.print(pixy.ccc.blocks[i].m_height); Serial.print(" | ");
    Serial.println(area);

    // Print colors
    if (sig == 1) Serial.println("THIS IS RED");
    if (sig == 2) Serial.println("THIS IS GREEN");
  }
  delay(100);
}
