#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin, Note *melody, uint16_t length) {
    this->pin = pin;
    this->melodyLength = length;
    this->melody = melody;
    pinMode(pin, OUTPUT);
}

void Buzzer::playMelody() {
    if (playing) return;
    startTime = millis();
    playing = true;
    currentNote = 0;
}

void Buzzer::stop() {
    playing = false;
    noTone(pin);
}

void Buzzer::tick(unsigned long time) {
    if (!playing) return;
    long delta = time - startTime;
    auto cursor = melody[currentNote];

    if (delta >= cursor.duration) {
        startTime = time - (delta - cursor.duration);
        currentNote = (currentNote + 1) % melodyLength;
        cursor = melody[currentNote];
    }

    auto nextCursor = melody[(currentNote + 1) % melodyLength];
    delta = time - startTime;

    auto newFreq = (long) cursor.frequency
                   + ((long) nextCursor.frequency - (long) cursor.frequency)
                     * (long) delta / (long) cursor.duration;

    tone(pin, newFreq);
}




