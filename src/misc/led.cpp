#include "led.h"

Led::Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN) : rPin(R_PIN), gPin(G_PIN), bPin(B_PIN) {
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);
}

void Led::changeColor(const Color &color) {
    analogWrite(rPin, color.components.red);
    analogWrite(gPin, color.components.green);
    analogWrite(bPin, color.components.blue);
}

void Led::changeColor(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
}
