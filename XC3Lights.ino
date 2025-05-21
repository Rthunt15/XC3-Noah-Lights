#include <FastLED.h>

#define DATA_PIN 2
#define NUM_LEDS 69  // 45 + 12 + 12
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

#define RING45_START 0
#define RING45_COUNT 45
#define RING12A_START 45
#define RING12B_START 57
#define RING12_COUNT 12

CRGB leds[NUM_LEDS];

unsigned long lastPatternChange = 0;
const unsigned long patternInterval = 120000; // 2 minutes
uint8_t currentPattern = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
}

void loop() {
  if (millis() - lastPatternChange > patternInterval) {
    currentPattern = (currentPattern + 1) % 5; // Number of pattern sets
    lastPatternChange = millis();
  }

  switch (currentPattern) {
    case 0: rainbow45_pulse12(); break;
    case 1: swirl45_mirror12(); break;
    case 2: flashSyncAll(); break;
    case 3: wave45_inSync12(); break;
    case 4: sparkle45_mirrorChase12(); break;
  }

  FastLED.show();
  delay(30); // adjust for effect speed
}

// --- Pattern Definitions ---

void rainbow45_pulse12() {
  static uint8_t hue = 0;
  fill_rainbow(&leds[RING45_START], RING45_COUNT, hue++, 3);

  uint8_t brightness = beatsin8(30, 50, 255);
  fill_solid(&leds[RING12A_START], RING12_COUNT, CHSV(hue, 255, brightness));
  fill_solid(&leds[RING12B_START], RING12_COUNT, CHSV(hue, 255, brightness));
}

void swirl45_mirror12() {
  static uint8_t pos = 0;
  fadeToBlackBy(leds, NUM_LEDS, 20);
  leds[(RING45_START + pos) % RING45_COUNT] = CHSV(pos * 5, 255, 255);
  
  // Mirror effect on 12-rings
  uint8_t index = pos % RING12_COUNT;
  leds[RING12A_START + index] = CHSV(pos * 7, 255, 255);
  leds[RING12B_START + (RING12_COUNT - 1 - index)] = CHSV(pos * 7, 255, 255);
  
  pos++;
}

void flashSyncAll() {
  uint8_t flashHue = millis() / 10;
  CRGB color = CHSV(flashHue, 255, (millis() / 500) % 2 == 0 ? 255 : 0);
  fill_solid(leds, NUM_LEDS, color);
}

void wave45_inSync12() {
  static uint8_t wavePos = 0;
  for (int i = 0; i < RING45_COUNT; i++) {
    leds[RING45_START + i] = CHSV(160, 255, sin8((i * 10) + wavePos));
  }

  for (int i = 0; i < RING12_COUNT; i++) {
    uint8_t val = sin8((i * 20) + wavePos);
    leds[RING12A_START + i] = CHSV(0, 255, val);
    leds[RING12B_START + i] = CHSV(0, 255, val);
  }
  wavePos++;
}

void sparkle45_mirrorChase12() {
  fadeToBlackBy(leds, NUM_LEDS, 10);

  // Sparkle random LED in 45-ring
  int index = RING45_START + random(RING45_COUNT);
  leds[index] = CHSV(random8(), 200, 255);

  // Chasing dot mirrored
  static uint8_t chase = 0;
  leds[RING12A_START + chase] = CHSV(128, 255, 255);
  leds[RING12B_START + (RING12_COUNT - 1 - chase)] = CHSV(128, 255, 255);
  chase = (chase + 1) % RING12_COUNT;
}
