#ifndef Animation_H_
#define Animation_H_

#include "WS2812.h"
#include <inttypes.h>

enum ANIMATION {
    FIRE,
    POLICE
};

class Animation {
public:
    void fire(WS2812 *ledStrip, uint16_t time);

    void fireFront(WS2812 *ledStrip, uint16_t time);

    void police(WS2812 *ledStrip, uint16_t time);

    void policeFront(WS2812 *ledStrip, uint16_t time);

private:
    uint16_t rainbow_hue;
};

#endif
