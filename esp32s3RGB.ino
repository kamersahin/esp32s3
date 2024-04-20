#include <FastLED.h>

#define LED_PIN     38
#define NUM_LEDS    1

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);  // RGB LED'in bağlı olduğu pin ve LED tipini belirtin
}

void loop() {
  // LED'i kırmızı olarak ayarla ve 1 saniye beklet
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(1000);

  // LED'i yeşil olarak ayarla ve 1 saniye beklet
  leds[0] = CRGB::Yellow;
  FastLED.show();
  delay(1000);

  // LED'i mavi olarak ayarla ve 1 saniye beklet
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(1000);

    // LED'i beyaz olarak ayarla ve 1 saniye beklet
  leds[0] = CRGB::Yellow;
  FastLED.show();
  delay(1000);


}
