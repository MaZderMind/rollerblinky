#include "config.h"
#include "WS2812.h"

#include <util/delay.h>

#define SATURATION 255
#define VALUE 127

#define LED_COUNT_LONG 13
#define LED_COUNT_SHORT 5

WS2812 ledA(LED_COUNT_LONG);
WS2812 ledB(LED_COUNT_SHORT);
WS2812 ledC(LED_COUNT_LONG);
cRGB color;

void setup() {
  ledA.setOutput(&PORTH, &DDRH, PH4);
  ledB.setOutput(&PORTE, &DDRE, PE3);
  ledC.setOutput(&PORTE, &DDRE, PE4);
}

void loop() {
  for(int i = 0; i < LED_COUNT_LONG; i++)
  {
    color.SetRGB(255, (i % 2) * 255, 0);
    ledA.set_crgb_at(i, color);
  }

  for(int i = 0; i < LED_COUNT_SHORT; i++)
  {
    color.SetRGB(0, 255, (i % 2) * 255);
    ledB.set_crgb_at(i, color);
  }


  for(int i = 0; i < LED_COUNT_LONG; i++)
  {
    color.SetRGB((i % 2 == 0) ? 0 : 255, 0, (i % 2 == 0) ? 255 : 64);
    ledC.set_crgb_at(i, color);
  }

  ledA.sync();
  ledB.sync();
  ledC.sync();

  _delay_ms(500);
}

int main() {
  setup();
  while(true) {
    loop();
  }

  return 0;
}
