#pragma once

#include <Arduino.h>

struct Note {
    int frequency;
    unsigned long duration;
};

class Buzzer {
private:
    Note *melody;
    uint8_t pin;
    uint16_t melodyLength;
    bool playing;
    unsigned long startTime;
    uint16_t currentNote;
public:
    explicit Buzzer(uint8_t pin, Note *melody, uint16_t length);
    void playMelody();
    void tick(unsigned long time);
    void stop();
};
