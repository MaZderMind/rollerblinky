//
// Created by peter on 22.12.18.
//

#include "Debounce.h"
#include "bits.h"
#include <avr/io.h>


Debounce::Debounce(volatile const uint8_t *port, volatile uint8_t *reg, uint8_t pin) {
    this->port = port;
    this->pin = pin;
    SETBIT(*reg, pin);
}

void Debounce::Tick() {
    if (BITSET(*port, pin)) {
        if (pressedCounter <= Debounce::COUNTER_THRESHOLD) {
            pressedCounter++;
        }
    } else {
        pressedCounter = 0;
    }
}

bool Debounce::IsPressed() {
    return pressedCounter >= Debounce::COUNTER_THRESHOLD;
}

bool Debounce::IsClicked() {
    if (IsPressed()) {
        if (!isClicked) {
            isClicked = true;
            return true;
        } else {
            return false;
        }
    } else {
        isClicked = false;
        return false;
    }
}
