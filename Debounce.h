#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <inttypes.h>

class Debounce {
public:
    Debounce(const volatile uint8_t *port, volatile uint8_t *reg, uint8_t pin);

    void Tick();

    bool IsPressed();

    bool IsPressedLong();

    bool IsClicked();

    void TakeClick();

private:
    uint8_t pressedCounter = 0;
    static const uint8_t COUNTER_THRESHOLD = 2;
    static const uint8_t COUNTER_THRESHOLD_LONG = 20;

    bool isClicked = false;
    bool clickTaken = false;

    const volatile uint8_t *port;
    uint8_t pin;
};


#endif //DEBOUNCE_H
