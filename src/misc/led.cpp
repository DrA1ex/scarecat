//
// Created by dp on 27.06.2024.
//

#include "led.h"

Led::Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN) {
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
    this->rPin = R_PIN;
    this->gPin = G_PIN;
    this->bPin = B_PIN;
}

void Led::changeColor(Color color) {
    analogWrite(this->rPin, color.red);
    analogWrite(this->gPin, color.green);
    analogWrite(this->bPin, color.blue);
}

void Led::changeColor(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(this->rPin, r);
    analogWrite(this->gPin, g);
    analogWrite(this->bPin, b);
}


