#include "Animation.h"
#include <stdlib.h>

void Animation::fire(WS2812 *ledStrip, uint16_t time) {
    uint16_t hue = rand() % 60;
    uint16_t val = rand() % 255;
    cRGB color;
    color.SetHSV(hue, DEFAULT_SATURATION, val);
    ledStrip->pushColor(color);
}

void Animation::fireFront(WS2812 *ledStrip, uint16_t time) {
    uint16_t hue = rand() % 60;
    cRGB color;
    color.SetHSV(hue);
    ledStrip->pushColor(color);
}

void Animation::police(WS2812 *ledStrip, uint16_t time) {
    cRGB color;
    uint8_t sequence = time % 64;
    if (sequence < 16) {
        if (sequence % 4 < 2) {
            color.SetRGB(0, 0, 255);
        } else {
            color.SetRGB(255, 0, 0);
        }
        ledStrip->fillColor(color);
    } else {
        if (sequence % 8 < 4) {
            color.SetRGB(0, 0, 255);
        } else {
            color.SetRGB(255, 0, 0);
        }

        ledStrip->pushColor(color);
    }
}

void Animation::policeFront(WS2812 *ledStrip, uint16_t time) {
    if (time % 4 != 0) {
        return;
    }

    cRGB color;

    if (time % 16 < 8) {
        color.SetRGB(0, 0, 255);
    } else {
        color.SetRGB(255, 0, 0);
    }

    ledStrip->pushColor(color);
}
