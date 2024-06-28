#pragma once

#include "Arduino.h"

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Led {

private:
    uint8_t rPin, gPin, bPin;
public:
    Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN);

    void changeColor(Color color);
    void changeColor(uint8_t r, uint8_t g, uint8_t b);

};
