#include "led.h"

Led::Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN) : _rPin(R_PIN), _gPin(G_PIN), _bPin(B_PIN) {
    pinMode(_rPin, OUTPUT);
    pinMode(_gPin, OUTPUT);
    pinMode(_bPin, OUTPUT);
}

void Led::changeColor(const Color &color) {
    analogWrite(_rPin, color.components.red);
    analogWrite(_gPin, color.components.green);
    analogWrite(_bPin, color.components.blue);
}

void Led::changeColor(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(_rPin, r);
    analogWrite(_gPin, g);
    analogWrite(_bPin, b);
}
