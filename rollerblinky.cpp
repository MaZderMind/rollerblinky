#include "config.h"
#include "WS2812.h"
#include "Animation.h"
#include "Debounce.h"
#include "bits.h"

#include <util/delay.h>

#define LED_COUNT_LONG 13
#define LED_COUNT_SHORT 5

WS2812 ledLeft(LED_COUNT_LONG);
WS2812 ledFront(LED_COUNT_SHORT);
WS2812 ledRight(LED_COUNT_LONG);

Debounce changeAnimationKey = Debounce(&PINF, &DDRF, PF4);

ANIMATION animation = ANIMATION::FIRE;

bool standby = false;

void setup() {
    ledLeft.setOutput(&PORTH, &DDRH, PH4);
    ledFront.setOutput(&PORTE, &DDRE, PE3);
    ledRight.setOutput(&PORTE, &DDRE, PE4);
}

void loop(uint16_t time) {
    changeAnimationKey.Tick();
    _delay_ms(1000 / 25);

    if (changeAnimationKey.IsPressedLong()) {
        standby = true;

        cRGB black(0, 0, 0);
        ledLeft.fillColor(black);
        ledFront.fillColor(black);
        ledRight.fillColor(black);

        ledLeft.sync();
        ledFront.sync();
        ledRight.sync();

        return;
    } else if (standby && changeAnimationKey.IsPressed()) {
        changeAnimationKey.TakeClick();
        standby = false;
        return;
    }
    if (standby) {
        return;
    }

    switch (animation) {
        case ANIMATION::FIRE:
            Animation::fire(&ledLeft);
            Animation::fireFront(&ledFront);
            Animation::fire(&ledRight);
            break;

        case ANIMATION::POLICE:
            Animation::police(&ledLeft, time);
            Animation::policeFront(&ledFront, time);
            Animation::police(&ledRight, time);
            break;

        case ANIMATION::RAINBOW:
            Animation::rainbow(&ledLeft, time);
            Animation::rainbow(&ledFront, time);
            Animation::rainbow(&ledRight, time);
            break;

        case ANIMATION::ZEBRA:
            Animation::zebra(&ledLeft, time);
            Animation::zebra(&ledFront, time);
            Animation::zebra(&ledRight, time);
            break;

        case ANIMATION::ALIEN:
            Animation::alien(&ledLeft, time, false);
            Animation::alien(&ledFront, time, false);
            Animation::alien(&ledRight, time, true);
            break;
    }

    if (changeAnimationKey.IsClicked()) {
        switch (animation) {
            case ANIMATION::FIRE:
                animation = ANIMATION::POLICE;
                break;

            case ANIMATION::POLICE:
                animation = ANIMATION::RAINBOW;
                break;

            case ANIMATION::RAINBOW:
                animation = ANIMATION::ZEBRA;
                break;

            case ANIMATION::ZEBRA:
                animation = ANIMATION::ALIEN;
                break;

            case ANIMATION::ALIEN:
                animation = ANIMATION::FIRE;
                break;
        }
    }

    ledLeft.sync();
    ledFront.sync();
    ledRight.sync();
}

int main() {
    setup();
    uint16_t time = 0;
    while (true) {
        loop(time++);
    }

    return 0;
}
