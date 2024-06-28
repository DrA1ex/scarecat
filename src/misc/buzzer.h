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

    bool _playing;

    unsigned long _startTime;
    size_t _currentNote;

public:
    explicit Buzzer(uint8_t pin, Note *melody, uint16_t length);

    void tick(unsigned long time);

    void play();
    void stop();
};
