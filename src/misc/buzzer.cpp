#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin, Note *melody, uint16_t length) : _pin(pin), _melody(melody), _melodyLength(length) {
    pinMode(pin, OUTPUT);
}

void Buzzer::play() {
    if (_playing) return;

    _startTime = millis();
    _playing = true;
    _currentNote = 0;
}

void Buzzer::stop() {
    if (!_playing) return;

    _playing = false;
    noTone(_pin);
}

void Buzzer::tick(unsigned long time) {
    if (!_playing) return;

    auto delta = time - _startTime;
    auto cursor = _melody[_currentNote];

    if (delta >= cursor.duration) {
        _startTime = time - (delta - cursor.duration);
        delta = time - _startTime;

        _currentNote = (_currentNote + 1) % _melodyLength;
        cursor = _melody[_currentNote];
    }

    auto nextCursor = _melody[(_currentNote + 1) % _melodyLength];

    auto freqDiff = nextCursor.frequency - (long) cursor.frequency;
    auto newFreq = cursor.frequency + freqDiff * (long) delta / cursor.duration;

    tone(_pin, newFreq);
}
