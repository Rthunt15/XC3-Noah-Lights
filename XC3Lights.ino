#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define NUM_LEDS 45
#define BRIGHTNESS 100
#define TAIL_LENGTH 10
#define COLOR_DELAY 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentPixel = 0;
int fillProgress = 0;
int breatheBrightness = 0;
int breatheDirection = 1;
unsigned long lastUpdate = 0;
int currentPattern = 0;
unsigned long patternStartTime = 0;
unsigned long patternDuration = 2 * 60 * 1000;  // 2 minutes

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  patternStartTime = millis();
}

void loop() {
  unsigned long now = millis();

  if (now - patternStartTime > patternDuration) {
    currentPattern = (currentPattern + 1) % 8;  // now 8 patterns
    patternStartTime = now;
    currentPixel = 0;
    fillProgress = 0;
    breatheBrightness = 0;
    strip.clear();
    strip.show();
  }

  if (now - lastUpdate > COLOR_DELAY) {
    lastUpdate = now;
    switch (currentPattern) {
      case 0: pattern_chaseForward(); break;
      case 1: pattern_chaseBackward(); break;
      case 2: pattern_bounce(); break;
      case 3: pattern_dualOpposite(); break;
      case 4: pattern_quadChase(); break;
      case 5: pattern_breathe(); break;
      case 6: pattern_fillMirror(); break;
      case 7: pattern_loadingWheel(); break;
    }
  }
}

void pattern_chaseForward() {
  strip.clear();
  for (int i = 0; i < TAIL_LENGTH; i++) {
    int pixel = (currentPixel - i + NUM_LEDS) % NUM_LEDS;
    int brightness = map(TAIL_LENGTH - i, 0, TAIL_LENGTH, 0, BRIGHTNESS);
    strip.setPixelColor(pixel, 0, 0, brightness);
  }
  strip.show();
  currentPixel = (currentPixel + 1) % NUM_LEDS;
}

void pattern_chaseBackward() {
  strip.clear();
  for (int i = 0; i < TAIL_LENGTH; i++) {
    int pixel = (currentPixel + i) % NUM_LEDS;
    int brightness = map(TAIL_LENGTH - i, 0, TAIL_LENGTH, 0, BRIGHTNESS);
    strip.setPixelColor(pixel, 0, 0, brightness);
  }
  strip.show();
  currentPixel = (currentPixel - 1 + NUM_LEDS) % NUM_LEDS;
}

void pattern_bounce() {
  static int direction = 1;
  strip.clear();
  for (int i = 0; i < TAIL_LENGTH; i++) {
    int pixel = currentPixel - (i * direction);
    if (pixel < 0) pixel += NUM_LEDS;
    if (pixel >= NUM_LEDS) pixel -= NUM_LEDS;
    int brightness = map(TAIL_LENGTH - i, 0, TAIL_LENGTH, 0, BRIGHTNESS);
    strip.setPixelColor(pixel, 0, 0, brightness);
  }
  strip.show();
  currentPixel += direction;
  if (currentPixel >= NUM_LEDS - 1 || currentPixel <= 0) direction = -direction;
}

void pattern_dualOpposite() {
  strip.clear();
  for (int i = 0; i < TAIL_LENGTH; i++) {
    int pos1 = (currentPixel - i + NUM_LEDS) % NUM_LEDS;
    int pos2 = (pos1 + NUM_LEDS / 2) % NUM_LEDS;
    int brightness = map(TAIL_LENGTH - i, 0, TAIL_LENGTH, 0, BRIGHTNESS);
    strip.setPixelColor(pos1, 0, 0, brightness);
    strip.setPixelColor(pos2, 0, 0, brightness);
  }
  strip.show();
  currentPixel = (currentPixel + 1) % NUM_LEDS;
}

void pattern_quadChase() {
  strip.clear();
  for (int i = 0; i < TAIL_LENGTH; i++) {
    int offset = (currentPixel - i + NUM_LEDS) % NUM_LEDS;
    int brightness = map(TAIL_LENGTH - i, 0, TAIL_LENGTH, 0, BRIGHTNESS);
    for (int j = 0; j < 4; j++) {
      int start = (j * NUM_LEDS) / 4;
      int pixel = (start + offset) % NUM_LEDS;
      strip.setPixelColor(pixel, 0, 0, brightness);
    }
  }
  strip.show();
  currentPixel = (currentPixel + 1) % (NUM_LEDS / 4);  // sync speed
}

void pattern_breathe() {
  breatheBrightness += breatheDirection * 5;
  if (breatheBrightness >= BRIGHTNESS || breatheBrightness <= 0) {
    breatheDirection *= -1;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, breatheBrightness);
  }
  strip.show();
}

void pattern_fillMirror() {
  strip.clear();
  int mid = NUM_LEDS / 2;
  for (int i = 0; i <= fillProgress; i++) {
    int left = (mid - i + NUM_LEDS) % NUM_LEDS;
    int right = (mid + i) % NUM_LEDS;
    strip.setPixelColor(left, 0, 0, BRIGHTNESS);
    strip.setPixelColor(right, 0, 0, BRIGHTNESS);
  }
  strip.show();
  fillProgress++;
  if (fillProgress > mid) fillProgress = 0;
}

void pattern_loadingWheel() {
  strip.setPixelColor(currentPixel, 0, 0, BRIGHTNESS);
  strip.show();
  currentPixel++;
  if (currentPixel >= NUM_LEDS) {
    currentPixel = 0;
    strip.clear();  // Reset for next cycle
  }
}
