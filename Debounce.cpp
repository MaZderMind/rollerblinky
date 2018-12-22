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
        if (pressedCounter < 255) {
            pressedCounter++;
        }
    } else {
        pressedCounter = 0;
    }
}

bool Debounce::IsPressed() {
    return pressedCounter >= Debounce::COUNTER_THRESHOLD;
}

bool Debounce::IsPressedLong() {
    return pressedCounter >= Debounce::COUNTER_THRESHOLD_LONG;
}

bool Debounce::IsClicked() {
    if (IsPressed()) {
        if (!isClicked) {
            isClicked = true;
            return false;
        } else {
            return false;
        }
    } else {
        if (isClicked) {
            isClicked = false;
            if (clickTaken) {
                clickTaken = false;
                return false;
            } else {
                return true;
            }
        }

        return false;
    }
}

void Debounce::TakeClick() {
    clickTaken = true;
}
