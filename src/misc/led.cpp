#include "led.h"

Led::Led(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN) : _rPin(R_PIN), _gPin(G_PIN), _bPin(B_PIN) {
    pinMode(_rPin, OUTPUT);
    pinMode(_gPin, OUTPUT);
    pinMode(_bPin, OUTPUT);
}

void Led::_refreshColor(bool active) {
    analogWrite(_rPin, active ? _color.components.red : 0);
    analogWrite(_gPin, active ? _color.components.green : 0);
    analogWrite(_bPin, active ? _color.components.blue : 0);
}

void Led::setColor(const Color &color) {
    setColor(color.components.red, color.components.green, color.components.blue);
}

void Led::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color.components.red = r;
    _color.components.green = g;
    _color.components.blue = b;

    _refreshColor(_active);
}

void Led::flash(unsigned long duration) {
    if (_active && _blinkCount == 0) return;

    _active = true;
    _start_time = millis();

    _flashDuration = duration;
    _blinkCount = 0;
    _blinkCountLeft = 0;
    _continuously = false;

    _refreshColor(true);
}

void Led::blink(uint8_t count, bool continuously) {
    if (_active && count == 0) {
        turnOff();
    } else if (_active) {
        _continuously = continuously;
        _blinkCount = count;
        _blinkCountLeft = min(_blinkCountLeft, _blinkCount);
    } else {
        _active = true;
        _start_time = millis();

        _flashDuration = 0;
        _blinkCount = count;
        _blinkCountLeft = count;
        _continuously = continuously;

        _refreshColor(true);
    }
}

void Led::turnOff() {
    if (!_active) return;

    _active = false;
    _refreshColor(false);
}

void Led::tick(unsigned long time) {
    if (!_active) return;

    auto delta = time - _start_time;

    if (_blinkCountLeft > 0) {      // Blink Mode
        if (delta < LED_BLINK_ACTIVE_DURATION) {
            _refreshColor(true);
        } else {
            _refreshColor(false);

            if (delta - LED_BLINK_ACTIVE_DURATION > LED_BLINK_WAIT_DURATION) {
                _start_time = time;
                if (--_blinkCountLeft == 0 && !_continuously) turnOff();
            }
        }
    } else if (_blinkCount > 0 && _continuously) {   // Blink Mode Cool down
        if (delta > LED_BLINK_COOL_DOWN_DURATION) {
            _start_time = time;
            _blinkCountLeft = _blinkCount;
        }
    } else {                     // Flash Mode
        if (_flashDuration > 0 && delta < _flashDuration) {
            turnOff();
        }
    }
}
