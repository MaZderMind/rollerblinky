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
  ledC.setOutput(&PORTE, &DDRE, PE0);
}

void loop() {
  color.r = 255;
  color.g = 0;
  color.b = 0;
  for(int i = 0; i < LED_COUNT_LONG; i++)
  {
      ledA.set_crgb_at(i, color);
  }

  color.r = 0;
  color.g = 255;
  color.b = 0;
  for(int i = 0; i < LED_COUNT_SHORT; i++)
  {
      ledB.set_crgb_at(i, color);
  }


  color.r = 0;
  color.g = 0;
  color.b = 255;
  for(int i = 0; i < LED_COUNT_LONG; i++)
  {
      ledC.set_crgb_at(i, color);
  }

  ledA.sync();
  ledB.sync();
  ledC.sync();

  _delay_ms(500);
}

int clampHue(int hue)
{
  if(hue > 360)
  {
      hue %= 360;
  }
  return hue;
}

int main() {
  setup();
  while(true) {
    loop();
  }

  return 0;
}
