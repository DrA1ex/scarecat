#pragma once

#include "Arduino.h"

union Color {
    struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } components;

    uint32_t value;
};

class Led {
    uint8_t _rPin, _gPin, _bPin;

public:
    Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN);

    void changeColor(const Color &color);
    void changeColor(uint8_t r, uint8_t g, uint8_t b);
};
