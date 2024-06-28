#pragma once

#include <Arduino.h>

struct Note {
    int frequency;
    uint16_t duration;
};

class Buzzer {
    uint8_t _pin;
    Note *_melody;
    uint16_t _melodyLength;

    bool _playing = false;
    unsigned long _startTime = 0;
    size_t _currentNote = 0;

public:
    explicit Buzzer(uint8_t pin, Note *melody, uint16_t length);

    void tick(unsigned long time);

    void play();
    void stop();
};
