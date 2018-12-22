#include "config.h"
#include "WS2812.h"
#include "Animation.h"
#include "Debounce.h"
#include "bits.h"

#include <util/delay.h>

#define LED_COUNT_LONG 13
#define LED_COUNT_SHORT 5

WS2812 ledA(LED_COUNT_LONG);
WS2812 ledB(LED_COUNT_SHORT);
WS2812 ledC(LED_COUNT_LONG);

Debounce changeAnimationKey = Debounce(&PINF, &DDRF, PF4);

ANIMATION animation = ANIMATION::FIRE;
Animation animationRunner;

void setup() {
    ledA.setOutput(&PORTH, &DDRH, PH4);
    ledB.setOutput(&PORTE, &DDRE, PE3);
    ledC.setOutput(&PORTE, &DDRE, PE4);
}

void loop(uint16_t time) {
    switch (animation) {
        case ANIMATION::FIRE:
            animationRunner.fire(&ledA, time);
            animationRunner.fireFront(&ledB, time);
            animationRunner.fire(&ledC, time);
            break;

        case ANIMATION::POLICE:
            animationRunner.police(&ledA, time);
            animationRunner.policeFront(&ledB, time);
            animationRunner.police(&ledC, time);
            break;
    }

    if (changeAnimationKey.IsClicked()) {
        switch (animation) {
            case ANIMATION::FIRE:
                animation = ANIMATION::POLICE;
                break;

            case ANIMATION::POLICE:
                animation = ANIMATION::FIRE;
                break;
        }
    }

    ledA.sync();
    ledB.sync();
    ledC.sync();

    changeAnimationKey.Tick();

    _delay_ms(1000 / 25);
}

int main() {
    setup();
    uint16_t time = 0;
    while (true) {
        loop(time++);
    }

    return 0;
}
