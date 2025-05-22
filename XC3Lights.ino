#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define NUM_LEDS 69

#define RING_45_END 44
#define RING_12_1_START 45
#define RING_12_2_START 57

#define PATTERN_DURATION 120000  // 2 minutes

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastPatternChange = 0;
int currentPattern = 0;
const int totalPatterns = 4;


void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  unsigned long now = millis();
  if (now - lastPatternChange > PATTERN_DURATION) {
    currentPattern = (currentPattern + 1) % totalPatterns;
    lastPatternChange = now;
  }

  switch (currentPattern) {
    case 0:
      rotatingDotTail45();
      rotatingDot12s();
      break;
    case 1:
      sparkleFade45();
      mirroredWave12s();
      break;
    case 2:
      breatheRing45();
      breatheSync12s();
      break;
    case 3:
      multiDotRotate45();
      multiRotatingDot12s();
      break;
    case 4:
      pulseSync12s();
      breatheRing45();
      break;
    case 5:
      multi2Rotate45();
      multi2Rotate12s();
      break;
  }


  strip.show();
  delay(20);
}

// ------------ Core color ---------------
uint32_t lightBlue(uint8_t brightness) {
  return strip.Color(0, (brightness * 100) / 255, brightness);
}

// ------------ 45 LED Ring Patterns ------------

void rotatingDotTail45() {
  static int pos = 0;
  const int tailLength = 6;

  for (int i = 0; i <= RING_45_END; i++) {
    int distance = (i - pos + NUM_LEDS) % (RING_45_END + 1);
    if (distance < tailLength) {
      uint8_t brightness = 255 - (distance * (255 / tailLength));
      strip.setPixelColor(i, lightBlue(brightness));
    } else {
      strip.setPixelColor(i, 0);
    }
  }

  pos = (pos + 1) % (RING_45_END + 1);
}

void sparkleFade45() {
  for (int i = 0; i <= RING_45_END; i++) {
    uint8_t brightness = (random(15) == 0) ? 255 : random(10);
    strip.setPixelColor(i, lightBlue(brightness));
  }
}

void breatheRing45() {
  static float brightness = 0;
  static int direction = 1;

  brightness += 0.02f * direction;
  if (brightness >= 1.0 || brightness <= 0.1) direction *= -1;

  uint8_t b = (uint8_t)(brightness * 255);
  for (int i = 0; i <= RING_45_END; i++) {
    strip.setPixelColor(i, lightBlue(b));
  }
}

void multiDotRotate45() {
  static int pos = 0;
  const int tailLength = 6;
  const int spacing = 11;  // ~90° spacing on 45 LEDs

  int dots[] = {
    pos,
    (pos + spacing) % 45,
    (pos + 2 * spacing) % 45,
    (pos + 3 * spacing) % 45
  };

  for (int i = 0; i <= RING_45_END; i++) {
    strip.setPixelColor(i, 0);  // Clear
  }

  for (int d = 0; d < 4; d++) {
    for (int t = 0; t < tailLength; t++) {
      int index = (dots[d] - t + 45) % 45;
      uint8_t brightness = 255 - (t * (255 / tailLength));
      strip.setPixelColor(index, lightBlue(brightness));
    }
  }

  pos = (pos + 1) % 45;
}

void multi2Rotate45() {
  static int pos = 0;
  const int tailLength = 6;
  const int spacing = 22;  // ~180° spacing on 45 LEDs

  int dots[] = {
    pos,
    (pos + spacing) % 45,
  };

  for (int i = 0; i <= RING_45_END; i++) {
    strip.setPixelColor(i, 0);  // Clear
  }

  for (int d = 0; d < 4; d++) {
    for (int t = 0; t < tailLength; t++) {
      int index = (dots[d] - t + 45) % 45;
      uint8_t brightness = 255 - (t * (255 / tailLength));
      strip.setPixelColor(index, lightBlue(brightness));
    }
  }

  pos = (pos + 1) % 45;
}
// ------------ 12 LED Rings ------------

void pulseSync12s() {
  static float brightness = 0;
  static int direction = 1;

  brightness += 0.03f * direction;
  if (brightness >= 1.0 || brightness <= 0.2) direction *= -1;

  uint8_t b = (uint8_t)(brightness * 255);
  for (int i = 0; i < 12; i++) {
    strip.setPixelColor(RING_12_1_START + i, lightBlue(b));
    strip.setPixelColor(RING_12_2_START + i, lightBlue(b));
  }
}

void mirroredWave12s() {
  static int wavePos = 0;
  const float speed = 0.5;

  for (int i = 0; i < 12; i++) {
    int phase = (int)(128 + 127 * sin((i + wavePos) * speed));
    uint8_t b = (uint8_t)((phase * 255) / 255);
    strip.setPixelColor(RING_12_1_START + i, lightBlue(b));
    strip.setPixelColor(RING_12_2_START + (11 - i), lightBlue(b));
  }

  wavePos++;
}

void breatheSync12s() {
  static float brightness = 0;
  static int direction = 1;

  brightness += 0.02f * direction;
  if (brightness >= 1.0 || brightness <= 0.1) direction *= -1;

  uint8_t b = (uint8_t)(brightness * 255);
  for (int i = 0; i < 12; i++) {
    strip.setPixelColor(RING_12_1_START + i, lightBlue(b));
    strip.setPixelColor(RING_12_2_START + i, lightBlue(b));
  }
}

void rotatingDot12s() {
  static int pos = 0;
  const int tailLength = 3;

  for (int i = 0; i < 12; i++) {
    strip.setPixelColor(RING_12_1_START + i, 0);
    strip.setPixelColor(RING_12_2_START + i, 0);
  }

  for (int t = 0; t < tailLength; t++) {
    int index1 = (pos - t + 12) % 12;
    int index2 = (pos - t + 12) % 12;
    uint8_t brightness = 255 - (t * (255 / tailLength));
    strip.setPixelColor(RING_12_1_START + index1, lightBlue(brightness));
    strip.setPixelColor(RING_12_2_START + index2, lightBlue(brightness));
  }

  pos = (pos + 1) % 12;
}

void multi2Rotate12s() {
  static int pos = 0;
  const int tailLength = 3;
  const int spacing = 6;  // ~180° spacing on 12 LEDs

  int dots[] = {
    pos,
    (pos + spacing) % 45,
  };

  for (int i = 0; i < 12; i++) {
    strip.setPixelColor(RING_12_1_START + i, 0);
    strip.setPixelColor(RING_12_2_START + i, 0);
  }

  for (int d = 0; d < 4; d++) {
    for (int t = 0; t < tailLength; t++) {
      int index = (dots[d] - t + 45) % 45;
      uint8_t brightness = 255 - (t * (255 / tailLength));
      strip.setPixelColor(index, lightBlue(brightness));
    }
  }

  pos = (pos + 1) % 45;
}

void multiRotatingDot12s() {
  static int pos = 0;
  const int tailLength = 1;
  const int spacing = 3;  // ~90° spacing on 12 LEDs

  int dots[] = {
    pos,
    (pos + spacing) % 45,
    (pos + 2 * spacing) % 45,
    (pos + 3 * spacing) % 45
  };

  for (int i = 0; i < 12; i++) {
    strip.setPixelColor(RING_12_1_START + i, 0);
    strip.setPixelColor(RING_12_2_START + i, 0);
  }

  for (int d = 0; d < 4; d++) {
    for (int t = 0; t < tailLength; t++) {
      int index = (dots[d] - t + 12) % 12;
      uint8_t brightness = 255 - (t * (255 / tailLength));
      strip.setPixelColor(index, lightBlue(brightness));
    }
  }

  pos = (pos + 1) % 45;
}
