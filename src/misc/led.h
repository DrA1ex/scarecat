#pragma once

#include <Arduino.h>

#include "const.h"

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

    Color _color = {.value = 0};

    bool _active = false;

    bool _continuously = false;
    uint8_t _blinkCount = 0;
    uint8_t _blinkCountLeft = 0;
    unsigned long _flashDuration = 0;

    unsigned long _start_time = 0;

    void _refreshColor(bool active);

public:
    Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN);

    void setColor(const Color &color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    void blink(uint8_t count = 1, bool continuously = false);
    void flash(unsigned long duration = 0);

    void turnOff();

    void tick(unsigned long time);

    inline bool active() const { return _active; }
    inline uint8_t blinkCount() const { return _blinkCount; }
    inline unsigned long flashDuration() const { return _flashDuration; }
};
