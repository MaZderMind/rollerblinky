#ifndef Animation_H_
#define Animation_H_

#include "WS2812.h"
#include <inttypes.h>

enum ANIMATION {
    FIRE,
    POLICE,
    RAINBOW,
    ZEBRA,
    ALIEN
};

class Animation {
public:
    static void fire(WS2812 *ledStrip);

    static void fireFront(WS2812 *ledStrip);

    static void police(WS2812 *ledStrip, uint16_t time);

    static void policeFront(WS2812 *ledStrip, uint16_t time);

    static void rainbow(WS2812 *ledStrip, uint16_t time);

    static void zebra(WS2812 *ledStrip, uint16_t time);

    static void alien(WS2812 *ledStrip, uint16_t time, bool reverse);
};

#endif
