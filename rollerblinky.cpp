#include "config.h"
#include "WS2812.h"
#include "Animation.h"

#include <util/delay.h>

#define LED_COUNT_LONG 13
#define LED_COUNT_SHORT 5

WS2812 ledA(LED_COUNT_LONG);
WS2812 ledB(LED_COUNT_SHORT);
WS2812 ledC(LED_COUNT_LONG);

ANIMATION animation = ANIMATION::POLICE;

void setup() {
    ledA.setOutput(&PORTH, &DDRH, PH4);
    ledB.setOutput(&PORTE, &DDRE, PE3);
    ledC.setOutput(&PORTE, &DDRE, PE4);
}

void loop(uint16_t time) {
    switch (animation) {
        case ANIMATION::FIRE:
            Animation::fire(&ledA, time);
            Animation::fireFront(&ledB, time);
            Animation::fire(&ledC, time);
            break;

        case ANIMATION::POLICE:
            Animation::police(&ledA, time);
            Animation::policeFront(&ledB, time);
            Animation::police(&ledC, time);
            break;
    }

    ledA.sync();
    ledB.sync();
    ledC.sync();

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
